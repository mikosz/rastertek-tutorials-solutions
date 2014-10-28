#ifndef _GRAPHICS_LIGHTINGVERTEXSHADER_HPP_
#define _GRAPHICS_LIGHTINGVERTEXSHADER_HPP_

#include <boost/filesystem/path.hpp>

#include <d3d11.h>
#include <d3dx10math.h>
#include <d3dx11async.h>

#include "VertexShader.hpp"
#include "../Device.hpp"
#include "../ShaderConstantsBuffer.hpp"
#include "utils/COMWrapper.hpp"

namespace tutorials {
namespace graphics {
namespace shaders {

class LightingVertexShader : public VertexShader {
public:

	struct MatrixBuffer {
		D3DXMATRIX world;
		D3DXMATRIX view;
		D3DXMATRIX projection;
	};

	struct CameraBuffer {
		D3DXVECTOR3 cameraPosition;

		float padding_;
	};

	struct Vertex {
		D3DXVECTOR3 position;
		D3DXVECTOR2 textureCoord;
		D3DXVECTOR3 normal;
		D3DXVECTOR3 tangent;
		D3DXVECTOR3 binormal;
	};

	void initialise(ID3D11Device* device, utils::COMWrapper<ID3D10Blob> shaderBuffer);

	void reset();

	void bind(
		ID3D11DeviceContext* deviceContext,
		const MatrixBuffer& matrixBuffer,
		const CameraBuffer& cameraBuffer
		);

private:

	utils::COMWrapper<ID3D11InputLayout> inputLayout_;

	ShaderConstantsBuffer matrixBuffer_;

	ShaderConstantsBuffer cameraBuffer_;

};

} // namespace shaders
} // namespace graphics
} // namespace tutorials

#endif /* _GRAPHICS_LIGHTINGVERTEXSHADER_HPP_ */
