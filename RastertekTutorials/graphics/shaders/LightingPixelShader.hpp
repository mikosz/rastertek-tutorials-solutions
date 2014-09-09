#ifndef _GRAPHICS_LIGHTINGPIXELSHADER_HPP_
#define _GRAPHICS_LIGHTINGPIXELSHADER_HPP_

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

class LightingPixelShader : public PixelShader {
public:

	struct LightBuffer {
		D3DXVECTOR4 ambientColour;
		D3DXVECTOR4 diffuseColour;
		D3DXVECTOR3 lightDirection;
		float specularPower;
		D3DXVECTOR4 specularColour;
	};

	static utils::COMWrapper<ID3D10Blob> compileShader(
		ID3D11Device* device,
		const boost::filesystem::path& path,
		const std::string& function
		);

	void initialise(ID3D11Device* device, utils::COMWrapper<ID3D10Blob> shaderBuffer);

	void reset();

	void bind(
		ID3D11DeviceContext* deviceContext,
		Texture* texture,
		const LightBuffer& lightBuffer
		);

private:

	utils::COMWrapper<ID3D11SamplerState> samplerState_;

	ShaderConstantsBuffer lightBuffer_;

};

} // namespace shaders
} // namespace graphics
} // namespace tutorials

#endif /* _GRAPHICS_LIGHTINGPIXELSHADER_HPP_ */