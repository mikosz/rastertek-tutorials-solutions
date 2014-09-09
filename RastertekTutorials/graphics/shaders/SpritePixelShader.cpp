#include "SpritePixelShader.hpp"

#include <D3DX11.h>

using namespace tutorials;
using namespace tutorials::graphics;
using namespace tutorials::graphics::shaders;

void SpritePixelShader::initialise(ID3D11Device* device, utils::COMWrapper<ID3D10Blob> shaderBuffer) {
	PixelShader::initialise(device, shaderBuffer);

	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(samplerDesc));

	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	if (FAILED(device->CreateSamplerState(&samplerDesc, &samplerState_.get()))) {
		std::runtime_error("Failed to create a sampler state");
	}
}

void SpritePixelShader::reset() {
	samplerState_.reset();

	PixelShader::reset();
}

void SpritePixelShader::bind(
	ID3D11DeviceContext* deviceContext,
	Texture* texture
	) {
	ID3D11ShaderResourceView* resource = texture->resource();
	deviceContext->PSSetShaderResources(0, 1, &resource);

	ID3D11SamplerState* sampler = samplerState_.get();
	deviceContext->PSSetSamplers(0, 1, &sampler);

	PixelShader::bind(deviceContext);
}
