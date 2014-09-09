#include "A3DCamera.hpp"

using namespace tutorials;
using namespace tutorials::graphics;

A3DCamera::A3DCamera(const Properties& properties) {
	reset(properties);
}

void A3DCamera::reset(const Properties& properties) {
	D3DXMatrixLookAtLH(&viewMatrix_, &properties.position, &properties.lookAt, &properties.up);
	D3DXMatrixPerspectiveFovLH(
		&projectionMatrix_,
		properties.fieldOfView,
		properties.aspectRatio,
		properties.nearPlane,
		properties.farPlane
		);
}
