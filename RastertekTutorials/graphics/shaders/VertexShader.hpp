#ifndef _GRAPHICS_VERTEXSHADER_HPP_
#define _GRAPHICS_VERTEXSHADER_HPP_

#include <boost/filesystem/path.hpp>

#include <d3d11.h>

#include "utils/COMWrapper.hpp"

namespace tutorials {
namespace graphics {
namespace shaders {

class VertexShader {
public:

	static utils::COMWrapper<ID3D10Blob> compileShader(
		ID3D11Device* device,
		const boost::filesystem::path& path,
		const std::string& function
		);

	virtual void initialise(ID3D11Device* device, utils::COMWrapper<ID3D10Blob> shaderBuffer);

	virtual void reset();

	virtual void bind(ID3D11DeviceContext* deviceContext);

protected:

	~VertexShader() {
	}

private:

	utils::COMWrapper<ID3D11VertexShader> shader_;

};

} // namespace shaders
} // namespace graphics
} // namespace tutorials

#endif /* _GRAPHICS_VERTEXSHADER_HPP_ */
