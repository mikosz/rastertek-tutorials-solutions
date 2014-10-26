#ifndef _AUDIO_AUDIO_HPP_
#define _AUDIO_AUDIO_HPP_

#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")

#include <Windows.h>
#include <mmsystem.h>
#include <dsound.h>

#include <boost/filesystem.hpp>

#include "system/Window.hpp"
#include "utils/COMWrapper.hpp"

namespace tutorials {
namespace audio {

class Audio {
public:

	~Audio() {
		shutdown();
	}

	void initialise(system::Window::Handle window);

	void shutdown();

	utils::COMWrapper<IDirectSoundBuffer8> loadWaveFile(const boost::filesystem::path& path);

	void play(IDirectSoundBuffer8* buffer);

private:

	utils::COMWrapper<IDirectSound8> directSound_;

	utils::COMWrapper<IDirectSoundBuffer> primarySoundBuffer_;

};

} // namespace audio
} // namespace tutorials

#endif /* _AUDIO_AUDIO_HPP_ */
