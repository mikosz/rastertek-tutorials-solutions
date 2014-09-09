#include "Camera.hpp"

using namespace tutorials;
using namespace tutorials::graphics;

D3DXVECTOR3 Camera::position() const {
	D3DXMATRIX viewToWorld;
	D3DXMatrixInverse(&viewToWorld, 0, &viewMatrix());

	D3DXVECTOR3 position(viewToWorld._41, viewToWorld._42, viewToWorld._43);
	return position;
}
