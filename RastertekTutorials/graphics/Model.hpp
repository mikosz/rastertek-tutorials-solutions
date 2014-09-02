#ifndef _GRAPHICS_MODEL_HPP_
#define _GRAPHICS_MODEL_HPP_

#include <vector>

#include <boost/filesystem/path.hpp>

#include "VertexShader.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "Texture.hpp"
#include "model_loaders/ModelData.hpp"

namespace tutorials {
namespace graphics {

class Device;
class Camera;

class Model {
public:

	void initialise(Device* device, const model_loaders::ModelData& data);

	void reset();

	void render(Device* device, const Camera& camera);

	void moveTo(float x, float y, float z) {
		D3DXMatrixTranslation(&worldMatrix_, x, y, z);
	}

	void moveBy(float x, float y, float z) {
		D3DXMATRIX translationMatrix;
		D3DXMatrixTranslation(&translationMatrix, x, y, z);

		D3DXMatrixMultiply(&worldMatrix_, &worldMatrix_, &translationMatrix);
	}

	void rotateBy(float yaw, float pitch, float roll) {
		D3DXMATRIX rotationMatrix;
		D3DXMatrixRotationYawPitchRoll(&rotationMatrix, yaw, pitch, roll);

		D3DXMatrixMultiply(&worldMatrix_, &rotationMatrix, &worldMatrix_);
	}

	D3DXMATRIX worldMatrix() const {
		return worldMatrix_;
	}

	Texture& texture() {
		return texture_;
	}

private:

	D3DXMATRIX worldMatrix_;

	VertexBuffer vertexBuffer_;

	IndexBuffer indexBuffer_;

	model_loaders::ModelData::Faces facesData_;

	Texture texture_;

};

} // namespace tutorials 
} // namespace graphics

#endif /* _GRAPHICS_MODEL_HPP_ */
