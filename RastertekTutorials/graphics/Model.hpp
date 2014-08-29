#ifndef _GRAPHICS_MODEL_HPP_
#define _GRAPHICS_MODEL_HPP_

#include <vector>

#include "VertexShader.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"

namespace tutorials {
namespace graphics {

class Device;
class Camera;

class Model {
public:

	struct Data {

		std::vector<VertexShader::Vertex> vertices;

		std::vector<boost::uint16_t> indices;

	};

	Model(Device* device, const Data& data);

	void render(Device* device, const Camera& camera);

	void moveTo(float x, float y, float z) {
		D3DXMatrixTranslation(&worldMatrix_, x, y, z);
	}

	const D3DXMATRIX& worldMatrix() const {
		return worldMatrix_;
	}

private:

	D3DXMATRIX worldMatrix_;

	graphics::VertexBuffer vertexBuffer_;

	graphics::IndexBuffer indexBuffer_;

};

} // namespace tutorials 
} // namespace graphics

#endif /* _GRAPHICS_MODEL_HPP_ */
