#ifndef _GRAPHICS_VERTEXSHADER_HPP_
#define _GRAPHICS_VERTEXSHADER_HPP_

#include <boost/filesystem/path.hpp>

#include <d3d11.h>
#include <d3dx10math.h>
#include <d3dx11async.h>

#include "Device.hpp"
#include "ShaderConstantsBuffer.hpp"
#include "utils/COMWrapper.hpp"

namespace tutorials {
namespace graphics {

class VertexShader {
public:

	struct MatrixBuffer {
		D3DXMATRIX world;
		D3DXMATRIX view;
		D3DXMATRIX projection;
	};

	struct Vertex {
		D3DXVECTOR3 position;
		D3DXVECTOR4 colour;
	};

	static utils::COMWrapper<ID3D10Blob> compileShader(
		ID3D11Device* device,
		const boost::filesystem::path& path,
		const std::string& function
		);

	void initialise(ID3D11Device* device, utils::COMWrapper<ID3D10Blob> shaderBuffer);

	void reset();

	void bind(ID3D11DeviceContext* deviceContext, const MatrixBuffer& matrixBuffer);

private:

	utils::COMWrapper<ID3D11VertexShader> shader_;

	utils::COMWrapper<ID3D11InputLayout> inputLayout_;

	ShaderConstantsBuffer matrixBuffer_;

};

} // namespace graphics
} // namespace tutorials

#endif /* _GRAPHICS_VERTEXSHADER_HPP_ */
