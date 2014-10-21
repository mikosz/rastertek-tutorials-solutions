#include "A3DCamera.hpp"

using namespace tutorials;
using namespace tutorials::graphics;

void A3DCamera::initialise(const Properties& properties) {
	D3DXMatrixLookAtLH(&viewMatrix_, &properties.position, &properties.lookAt, &properties.up);
	D3DXMatrixPerspectiveFovLH(
		&projectionMatrix_,
		properties.fieldOfView,
		properties.aspectRatio,
		properties.nearPlane,
		properties.farPlane
		);
}
