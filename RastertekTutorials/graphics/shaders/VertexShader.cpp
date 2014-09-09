#include "VertexShader.hpp"

#include <D3DX11.h>

using namespace tutorials;
using namespace tutorials::graphics;
using namespace tutorials::graphics::shaders;

utils::COMWrapper<ID3D10Blob> VertexShader::compileShader(
	ID3D11Device* device,
	const boost::filesystem::path& path,
	const std::string& function
	) {
	utils::COMWrapper<ID3D10Blob> shaderBuffer;
	utils::COMWrapper<ID3D10Blob> errorMessageBuffer;

	HRESULT result = D3DX11CompileFromFile(
		path.string().c_str(),
		0,
		0,
		function.c_str(),
		"vs_5_0",
		D3D10_SHADER_ENABLE_STRICTNESS,
		0,
		0,
		&shaderBuffer.get(),
		&errorMessageBuffer.get(),
		0
		);
	if (FAILED(result)) {
		if (errorMessageBuffer.get()) {
			std::string errorMessage(
				reinterpret_cast<const char*>(errorMessageBuffer->GetBufferPointer()),
				errorMessageBuffer->GetBufferSize()
				);
			throw std::runtime_error("Failed to compile shader file " + path.string() + ". Errors:\n" + errorMessage);
		} else {
			throw std::runtime_error("Failed to compile shader file " + path.string());
		}
	}

	return shaderBuffer;
}

void VertexShader::initialise(ID3D11Device* device, utils::COMWrapper<ID3D10Blob> shaderBuffer) {
	if (FAILED(device->CreateVertexShader(shaderBuffer->GetBufferPointer(), shaderBuffer->GetBufferSize(), 0, &shader_.get()))) {
		throw std::runtime_error("Failed to create a vertex shader");
	}
}

void VertexShader::reset() {
	shader_.reset();
}

void VertexShader::bind(ID3D11DeviceContext* deviceContext) {
	deviceContext->VSSetShader(shader_, 0, 0);
}
