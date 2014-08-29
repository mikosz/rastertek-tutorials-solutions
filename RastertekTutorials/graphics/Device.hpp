#ifndef _GRAPHICS_DEVICE_HPP_
#define _GRAPHICS_DEVICE_HPP_

#include <d3d11.h>

#include "system/Window.hpp"
#include "utils/COMWrapper.hpp"

namespace tutorials {
namespace graphics {

class Device {
public:

	static const DXGI_FORMAT DISPLAY_FORMAT = DXGI_FORMAT_R8G8B8A8_UNORM;

	struct Properties {

		size_t displayWidth;

		size_t displayHeight;

		bool fullscreen;

		bool vsync;

		system::Window::Handle windowHandle;

	};

	~Device() {
		shutdown();
	}

	void initialise(const Properties& properties);

	void shutdown();

	void beginScene();

	void endScene();

	ID3D11Device* d3dDevice() {
		return *d3dDevice_;
	}

	ID3D11DeviceContext* d3dDeviceContext() {
		return *d3dDeviceContext_;
	}

private:

	Properties currentProperties_;

	utils::COMWrapper<IDXGIAdapter> adapter_;

	utils::COMWrapper<ID3D11Device> d3dDevice_;

	utils::COMWrapper<ID3D11DeviceContext> d3dDeviceContext_;

	utils::COMWrapper<IDXGISwapChain> swapChain_;

	utils::COMWrapper<ID3D11RenderTargetView> renderTargetView_;

	utils::COMWrapper<ID3D11Texture2D> depthStencilBuffer_;

	utils::COMWrapper<ID3D11DepthStencilState> depthStencilState_;

	utils::COMWrapper<ID3D11DepthStencilView> depthStencilView_;

	utils::COMWrapper<ID3D11RasterizerState> rasteriserState_;

};

} // namespace graphics
} // namespace tutorials

#endif /* _GRAPHICS_DEVICE_HPP_ */
