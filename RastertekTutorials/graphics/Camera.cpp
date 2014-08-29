#include "Camera.hpp"

using namespace tutorials;
using namespace tutorials::graphics;

Camera::Camera(const Properties& properties) {
	reset(properties);
}

void Camera::reset(const Properties& properties) {
	D3DXMatrixLookAtLH(&viewMatrix_, &properties.position, &properties.lookAt, &properties.up);
	D3DXMatrixPerspectiveFovLH(
		&projectionMatrix_,
		properties.fieldOfView,
		properties.aspectRatio,
		properties.nearPlane,
		properties.farPlane
		);
}
