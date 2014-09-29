#include "A2DCamera.hpp"

using namespace tutorials;
using namespace tutorials::graphics;

A2DCamera::A2DCamera(const Properties& properties) {
	reset(properties);
}

void A2DCamera::reset(const Properties& properties) {
	D3DXMatrixIdentity(&viewMatrix_);
	viewMatrix_._41 = properties.position.x;
	viewMatrix_._42 = properties.position.y;
	viewMatrix_._43 = properties.position.z;
	D3DXMatrixInverse(&viewMatrix_, 0, &viewMatrix_);

	D3DXMatrixOrthoLH(
		&projectionMatrix_,
		static_cast<float>(properties.viewWidth),
		static_cast<float>(properties.viewHeight),
		properties.nearPlane,
		properties.farPlane
		);
}
