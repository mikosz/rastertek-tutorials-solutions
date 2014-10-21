#ifndef _GRAPHICS_A3DCAMERA_HPP_
#define _GRAPHICS_A3DCAMERA_HPP_

#include "Camera.hpp"

namespace tutorials {
namespace graphics {

class A3DCamera : public Camera {
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

	void initialise(const Properties& properties);

	const D3DXMATRIX& viewMatrix() const {
		return viewMatrix_;
	}

	const D3DXMATRIX& projectionMatrix() const {
		return projectionMatrix_;
	}

private:

	D3DXMATRIX viewMatrix_;

	D3DXMATRIX projectionMatrix_;

};

} // namespace graphics
} // namespace tutorials

#endif /* _GRAPHICS_A3DCAMERA_HPP_ */
