#ifndef _GRAPHICS_RENDERTARGETTEXTURE_HPP_
#define _GRAPHICS_RENDERTARGETTEXTURE_HPP_

#include <d3d11.h>

#include "utils/COMWrapper.hpp"

namespace tutorials {
namespace graphics {

class RenderTargetTexture {
public:

	void initialise(ID3D11Device* device, size_t width, size_t height);

	void reset();

	void use(ID3D11DeviceContext* deviceContext, ID3D11DepthStencilView* depthStencilView);

	void clear(ID3D11DeviceContext* deviceContext, ID3D11DepthStencilView* depthStencilView);

	utils::COMWrapper<ID3D11ShaderResourceView> shaderResourceView() {
		return shaderResourceView_;
	}

private:

	utils::COMWrapper<ID3D11Texture2D> texture_;

	utils::COMWrapper<ID3D11RenderTargetView> renderTargetView_;

	utils::COMWrapper<ID3D11ShaderResourceView> shaderResourceView_;

};

} // namespace graphics
} // namespace tutorials

#endif /* _GRAPHICS_RENDERTARGETTEXTURE_HPP_ */
