#include "Texture.hpp"

#include <D3DX11tex.h>

using namespace tutorials;
using namespace tutorials::graphics;

void Texture::initialise(ID3D11Device* device, const boost::filesystem::path& textureFile) {
	if (FAILED(D3DX11CreateShaderResourceViewFromFile(device, textureFile.string().c_str(), 0, 0, &texture_.get(), 0))) {
		throw std::runtime_error("Failed to create shader resource view from " + textureFile.string());
	}
}

void Texture::initialise(utils::COMWrapper<ID3D11ShaderResourceView> shaderResourceView) {
	texture_ = shaderResourceView;
}

void Texture::reset() {
	texture_.reset();
}
