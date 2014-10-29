#include "RenderTargetTexture.hpp"

#include <cstring>
#include <stdexcept>

using namespace tutorials;
using namespace tutorials::graphics;

void RenderTargetTexture::initialise(ID3D11Device* device, size_t width, size_t height) {
	D3D11_TEXTURE2D_DESC textureDesc;
	std::memset(&textureDesc, 0, sizeof(textureDesc));

	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

	if (FAILED(device->CreateTexture2D(&textureDesc, 0, &texture_.get()))) {
		throw std::runtime_error("Failed to create a render target texture");
	}

	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	std::memset(&renderTargetViewDesc, 0, sizeof(renderTargetViewDesc));

	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

	if (FAILED(device->CreateRenderTargetView(texture_, &renderTargetViewDesc, &renderTargetView_.get()))) {
		throw std::runtime_error("Failed to create a render target view");
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	std::memset(&shaderResourceViewDesc, 0, sizeof(shaderResourceViewDesc));

	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	if (FAILED(device->CreateShaderResourceView(texture_, &shaderResourceViewDesc, &shaderResourceView_.get()))) {
		throw std::runtime_error("Failed to create a shader resource view");
	}
}

void RenderTargetTexture::reset() {
	shaderResourceView_.reset();
	renderTargetView_.reset();
	texture_.reset();
}

void RenderTargetTexture::use(ID3D11DeviceContext* deviceContext, ID3D11DepthStencilView* depthStencilView) {
	deviceContext->OMSetRenderTargets(1, &renderTargetView_.get(), depthStencilView);
}

void RenderTargetTexture::clear(ID3D11DeviceContext* deviceContext, ID3D11DepthStencilView* depthStencilView) {
	float clearColour[] = { 0.5f, 0.5f, 1.0f, 1.0f };

	deviceContext->ClearRenderTargetView(renderTargetView_, clearColour);
	deviceContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}
