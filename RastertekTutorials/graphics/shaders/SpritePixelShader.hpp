#ifndef _GRAPHICS_SPRITEPIXELSHADER_HPP_
#define _GRAPHICS_SPRITEPIXELSHADER_HPP_

#include <string>

#include <boost/filesystem/path.hpp>

#include <d3d11.h>
#include <D3DX10math.h>

#include "PixelShader.hpp"
#include "../Texture.hpp"
#include "../ShaderConstantsBuffer.hpp"
#include "utils/COMWrapper.hpp"

namespace tutorials {
namespace graphics {
namespace shaders {

class SpritePixelShader : public PixelShader {
public:

	void initialise(ID3D11Device* device, utils::COMWrapper<ID3D10Blob> shaderBuffer);

	void reset();

	void bind(
		ID3D11DeviceContext* deviceContext,
		Texture* texture
		);

private:

	utils::COMWrapper<ID3D11SamplerState> samplerState_;

};

} // namespace shaders
} // namespace graphics
} // namespace tutorials

#endif /* _GRAPHICS_SPRITEPIXELSHADER_HPP_ */
