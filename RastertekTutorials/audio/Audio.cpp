#include "Audio.hpp"

#include <cstdint>
#include <stdexcept>
#include <fstream>

using namespace tutorials;
using namespace tutorials::audio;

namespace /* anonymous */ {

struct WaveFileHeader
{
	char chunkId[4];
	std::uint32_t chunkSize;
	char format[4];
};

struct WaveFileFmtChunkHeader {
	char subChunkId[4];
	std::uint32_t subChunkSize;
	std::uint16_t audioFormat;
	std::uint16_t numChannels;
	std::uint32_t sampleRate;
	std::uint32_t bytesPerSecond;
	std::uint16_t blockAlign;
	std::uint16_t bitsPerSample;
};

struct WaveFileDataChunkHeader {
	char dataChunkId[4];
	std::uint32_t dataSize;
};

} // anonymous namespace

void Audio::initialise(system::Window::Handle window) {
	if (FAILED(DirectSoundCreate8(0, &directSound_.get(), 0))) {
		throw std::runtime_error("Failed to initialise direct sound");
	}

	if (FAILED(directSound_->SetCooperativeLevel(window, DSSCL_PRIORITY))) {
		throw std::runtime_error("Failed to set cooperative level for direct sound");
	}

	{
		DSBUFFERDESC bufferDesc;
		std::memset(&bufferDesc, 0, sizeof(bufferDesc));

		bufferDesc.dwSize = sizeof(bufferDesc);
		bufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME;
		bufferDesc.guid3DAlgorithm = GUID_NULL;

		if (FAILED(directSound_->CreateSoundBuffer(&bufferDesc, &primarySoundBuffer_.get(), 0))) {
			throw std::runtime_error("Failed to create the primary sound buffer");
		}

		WAVEFORMATEX waveFormat;
		std::memset(&waveFormat, 0, sizeof(waveFormat));

		waveFormat.wFormatTag = WAVE_FORMAT_PCM;
		waveFormat.nSamplesPerSec = 44100;
		waveFormat.wBitsPerSample = 16;
		waveFormat.nChannels = 2;
		waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
		waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;

		if (FAILED(primarySoundBuffer_->SetFormat(&waveFormat))) {
			throw std::runtime_error("Failed to set the primary sound buffer format");
		}
	}
}

void Audio::shutdown() {
	primarySoundBuffer_.reset();
	directSound_.reset();
}

utils::COMWrapper<IDirectSoundBuffer8> Audio::loadWaveFile(const boost::filesystem::path& path) {
	std::ifstream ifs(path.string().c_str(), std::ios::binary);
	ifs.exceptions(std::ios::badbit);

	WaveFileHeader header;
	ifs.read(reinterpret_cast<char*>(&header), sizeof(header));

	WaveFileFmtChunkHeader fmtHeader;
	ifs.read(reinterpret_cast<char*>(&fmtHeader), sizeof(fmtHeader));

	if (
		(std::strncmp(header.chunkId, "RIFF", 4) != 0) ||
		(std::strncmp(header.format, "WAVE", 4) != 0) ||
		(std::strncmp(fmtHeader.subChunkId, "fmt ", 4) != 0) ||
		(fmtHeader.audioFormat != WAVE_FORMAT_PCM) ||
		(fmtHeader.numChannels != 2) ||
		(fmtHeader.sampleRate != 44100) ||
		(fmtHeader.bitsPerSample != 16)
		) {
		throw std::runtime_error("Bad wave file header");
	}

	size_t padding = fmtHeader.subChunkSize - (sizeof(WaveFileFmtChunkHeader) - offsetof(WaveFileFmtChunkHeader, audioFormat));

	if (padding > 0) {
		ifs.ignore(padding);
	}

	WaveFileDataChunkHeader dataHeader;
	ifs.read(reinterpret_cast<char*>(&dataHeader), sizeof(dataHeader));

	if (std::strncmp(dataHeader.dataChunkId, "data", 4) != 0) {
		throw std::runtime_error("Bad data header");
	}

	WAVEFORMATEX waveFormat;
	std::memset(&waveFormat, 0, sizeof(waveFormat));

	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nSamplesPerSec = 44100;
	waveFormat.wBitsPerSample = 16;
	waveFormat.nChannels = 2;
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	waveFormat.cbSize = 0;

	utils::COMWrapper<IDirectSoundBuffer8> buffer;

	{
		DSBUFFERDESC bufferDesc;
		std::memset(&bufferDesc, 0, sizeof(bufferDesc));

		bufferDesc.dwSize = sizeof(bufferDesc);
		bufferDesc.dwFlags = DSBCAPS_CTRLVOLUME;
		bufferDesc.dwBufferBytes = dataHeader.dataSize;
		bufferDesc.dwReserved = 0;
		bufferDesc.lpwfxFormat = &waveFormat;
		bufferDesc.guid3DAlgorithm = GUID_NULL;

		utils::COMWrapper<IDirectSoundBuffer> tempBuffer;
		if (FAILED(directSound_->CreateSoundBuffer(&bufferDesc, &tempBuffer.get(), 0))) {
			throw std::runtime_error("Failed to create a sound buffer for wave data");
		}

		if (FAILED(tempBuffer->QueryInterface(IID_IDirectSoundBuffer8, reinterpret_cast<void**>(&buffer.get())))) {
			throw std::runtime_error("Failed to get IDirectSoundBuffer8 interface from IDirectSoundBuffer");
		}
	}

	char* data;
	DWORD dataSize;
	if (FAILED(buffer->Lock(0, dataHeader.dataSize, reinterpret_cast<void**>(&data), &dataSize, 0, 0, 0))) {
		throw std::runtime_error("Failed to lock data buffer");
	}

	ifs.read(data, dataHeader.dataSize);

	if (!ifs) {
		throw std::runtime_error("Failed to read wave data");
	}

	if (FAILED(buffer->Unlock(data, dataSize, 0, 0))) {
		throw std::runtime_error("Failed to unlock data buffer");
	}

	return buffer;
}

void Audio::play(IDirectSoundBuffer8* buffer) {
	if (FAILED(buffer->SetCurrentPosition(0))) {
		throw std::runtime_error("Failed to set audio buffer position");
	}

	if (FAILED(buffer->SetVolume(DSBVOLUME_MAX))) {
		throw std::runtime_error("Failed to set audio volume");
	}

	if (FAILED(buffer->Play(0, 0, 0))) {
		throw std::runtime_error("Failed to play audio buffer");
	}
}
