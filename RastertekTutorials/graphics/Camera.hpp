#ifndef _GRAPHICS_CAMERA_HPP_
#define _GRAPHICS_CAMERA_HPP_

#include <D3DX10math.h>

namespace tutorials {
namespace graphics {

class Camera {
public:

	struct Properties {

		D3DXVECTOR3 position;

		D3DXVECTOR3 up;

		D3DXVECTOR3 lookAt;

		float fieldOfView;

		float aspectRatio;

		float nearPlane;
		
		float farPlane;

	};

	Camera(const Properties& properties);

	void reset(const Properties& properties);

	const D3DXMATRIX& viewMatrix() const {
		return viewMatrix_;
	}

	const D3DXMATRIX& projectionMatrix() const {
		return projectionMatrix_;
	}

	const D3DXVECTOR3& position() const {
		return currentProperties_.position;
	}

private:

	Properties currentProperties_;

	D3DXMATRIX viewMatrix_;

	D3DXMATRIX projectionMatrix_;

};

} // namespace graphics
} // namespace tutorials

#endif /* _GRAPHICS_CAMERA_HPP_ */
