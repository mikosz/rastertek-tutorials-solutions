#include "Device.hpp"

#include <vector>
#include <stdexcept>

using namespace tutorials;
using namespace tutorials::graphics;

namespace /* anonymous */ {

void queryAdapterAndRefreshRate(const Device::Properties& properties, utils::COMWrapper<IDXGIAdapter>* adapter, DXGI_RATIONAL* refreshRate) {
	HRESULT result;

	utils::COMWrapper<IDXGIFactory> factory;
	if (FAILED(CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&factory.get())))) {
		throw std::runtime_error("Failed to create a DXGIFactory");
	}

	result = factory->EnumAdapters(0, &adapter->get());
	if (result == DXGI_ERROR_NOT_FOUND) {
		throw std::runtime_error("No video cards found");
	} else if (FAILED(result)) {
		throw std::runtime_error("Failed to enumerate video cards");
	}

	utils::COMWrapper<IDXGIOutput> output;
	result = (*adapter)->EnumOutputs(0, &output.get());
	if (result == DXGI_ERROR_NOT_FOUND) {
		throw std::runtime_error("No video outputs found");
	} else if (FAILED(result)) {
		throw std::runtime_error("Failed to enumerate outputs");
	}

	std::vector<DXGI_MODE_DESC> displayModes;
	UINT modeCount;
	result = output->GetDisplayModeList(Device::DISPLAY_FORMAT, DXGI_ENUM_MODES_INTERLACED, &modeCount, 0);
	if (FAILED(result)) {
		throw std::runtime_error("Failed to get display modes");
	}

	displayModes.resize(modeCount);
	result = output->GetDisplayModeList(Device::DISPLAY_FORMAT, DXGI_ENUM_MODES_INTERLACED, &modeCount, &displayModes.front());
	if (FAILED(result)) {
		throw std::runtime_error("Failed to get display modes");
	}

	assert(modeCount == displayModes.size());

	refreshRate->Numerator = 0;
	refreshRate->Denominator = 0;

	for (size_t i = 0; i< displayModes.size(); ++i) {
		if (displayModes[i].Width == properties.displayWidth && displayModes[i].Height == properties.displayHeight) {
			*refreshRate = displayModes[i].RefreshRate;
		}
	}
}

void createD3DDevice(
	const Device::Properties& properties,
	const DXGI_RATIONAL& refreshRate,
	utils::COMWrapper<IDXGISwapChain>* swapChain,
	utils::COMWrapper<ID3D11Device>* device,
	utils::COMWrapper<ID3D11DeviceContext>* deviceContext
	) {
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	swapChainDesc.BufferCount = 1;

	swapChainDesc.BufferDesc.Width = properties.displayWidth;
	swapChainDesc.BufferDesc.Height = properties.displayHeight;
	swapChainDesc.BufferDesc.Format = Device::DISPLAY_FORMAT;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

	if (properties.vsync) {
		swapChainDesc.BufferDesc.RefreshRate = refreshRate;
	} else {
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	swapChainDesc.OutputWindow = properties.windowHandle;

	swapChainDesc.Windowed = !properties.fullscreen;

	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	HRESULT result = D3D11CreateDeviceAndSwapChain(
		0,
		D3D_DRIVER_TYPE_HARDWARE,
		0,
		0,
		0,
		0,
		D3D11_SDK_VERSION,
		&swapChainDesc,
		&swapChain->get(),
		&device->get(),
		0,
		&deviceContext->get()
		);
	if (FAILED(result)) {
		throw std::runtime_error("Failed to create a directx device");
	}
}

void createRenderTargetView(ID3D11Device* device, IDXGISwapChain* swapChain, utils::COMWrapper<ID3D11RenderTargetView>* renderTargetView) {
	utils::COMWrapper<ID3D11Texture2D> backBuffer;

	if (FAILED(swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<LPVOID*>(&backBuffer.get())))) {
		throw std::runtime_error("Failed to retrieve swap chain back buffer");
	}

	if (FAILED(device->CreateRenderTargetView(backBuffer, 0, &renderTargetView->get()))) {
		throw std::runtime_error("Failed to create a render target view");
	}
}

void createDepthBuffer(
	const Device::Properties& deviceProperties,
	ID3D11Device* device,
	ID3D11DeviceContext* deviceContext,
	utils::COMWrapper<ID3D11Texture2D>* depthStencilBuffer,
	utils::COMWrapper<ID3D11DepthStencilState>* depthStencilState
	) {
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	depthBufferDesc.Width = deviceProperties.displayWidth;
	depthBufferDesc.Height = deviceProperties.displayHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	if (FAILED(device->CreateTexture2D(&depthBufferDesc, 0, &depthStencilBuffer->get()))) {
		throw std::runtime_error("Failed to create a depth stencil buffer");
	}

	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xff;
	depthStencilDesc.StencilWriteMask = 0xff;

	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	if (FAILED(device->CreateDepthStencilState(&depthStencilDesc, &depthStencilState->get()))) {
		throw std::runtime_error("Failed to create a depth stencil state");
	}

	deviceContext->OMSetDepthStencilState(*depthStencilState, 1);
}

void createDepthStencilView(
	ID3D11Device* device,
	ID3D11DeviceContext* deviceContext,
	ID3D11Texture2D* depthStencilBuffer,
	ID3D11RenderTargetView* renderTargetView,
	utils::COMWrapper<ID3D11DepthStencilView>* depthStencilView
	) {
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	if (FAILED(device->CreateDepthStencilView(depthStencilBuffer, &depthStencilViewDesc, &depthStencilView->get()))) {
		throw std::runtime_error("Failed to create a depth stencil view");
	}
}

void createRasteriserState(
	ID3D11Device* device,
	ID3D11DeviceContext* deviceContext,
	utils::COMWrapper<ID3D11RasterizerState>* rasteriserState
	) {
	D3D11_RASTERIZER_DESC rasteriserDesc;
	ZeroMemory(&rasteriserDesc, sizeof(rasteriserDesc));

	rasteriserDesc.AntialiasedLineEnable = false;
	rasteriserDesc.CullMode = D3D11_CULL_BACK;
	rasteriserDesc.DepthBias = 0;
	rasteriserDesc.DepthBiasClamp = 0.0f;
	rasteriserDesc.DepthClipEnable = true;
	rasteriserDesc.FillMode = D3D11_FILL_SOLID;
	rasteriserDesc.FrontCounterClockwise = false;
	rasteriserDesc.MultisampleEnable = false;
	rasteriserDesc.ScissorEnable = false;
	rasteriserDesc.SlopeScaledDepthBias = 0.0f;

	if (FAILED(device->CreateRasterizerState(&rasteriserDesc, &rasteriserState->get()))) {
		throw std::runtime_error("Failed to create a rasteriser state");
	}

	deviceContext->RSSetState(*rasteriserState);
}

void setupViewport(const Device::Properties& properties, ID3D11DeviceContext* deviceContext) {
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(viewport));

	viewport.Height = static_cast<float>(properties.displayHeight);
	viewport.Width = static_cast<float>(properties.displayWidth);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	deviceContext->RSSetViewports(1, &viewport);
}

} // anonymous namespace

void Device::initialise(const Properties& properties) {
	currentProperties_ = properties;

	DXGI_RATIONAL refreshRate;
	queryAdapterAndRefreshRate(properties, &adapter_, &refreshRate);
	
	createD3DDevice(properties, refreshRate, &swapChain_, &d3dDevice_, &d3dDeviceContext_);

	createRenderTargetView(d3dDevice_, swapChain_, &renderTargetView_);
	createDepthBuffer(properties, d3dDevice_, d3dDeviceContext_, &depthStencilBuffer_, &depthStencilState_);
	createDepthStencilView(d3dDevice_, d3dDeviceContext_, depthStencilBuffer_, renderTargetView_, &depthStencilView_);

	createRasteriserState(d3dDevice_, d3dDeviceContext_, &rasteriserState_);

	setupViewport(properties, d3dDeviceContext_);
}

void Device::shutdown() {
	rasteriserState_.reset();
	depthStencilView_.reset();
	depthStencilState_.reset();
	depthStencilBuffer_.reset();
	renderTargetView_.reset();
	swapChain_.reset();
	d3dDeviceContext_.reset();
	d3dDevice_.reset();
	adapter_.reset();
}

void Device::beginScene() {
	float colour[] = { 0.2f, 0.5f, 0.2f, 1.0f };

	d3dDeviceContext_->OMSetRenderTargets(1, &renderTargetView_.get(), *depthStencilView_);
	d3dDeviceContext_->ClearRenderTargetView(renderTargetView_, colour);
	d3dDeviceContext_->ClearDepthStencilView(depthStencilView_, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void Device::endScene() {
	swapChain_->Present(currentProperties_.vsync, 0);
}
