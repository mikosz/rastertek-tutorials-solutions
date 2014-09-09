#ifndef _GRAPHICS_CAMERA_HPP_
#define _GRAPHICS_CAMERA_HPP_

#include <D3DX10math.h>

namespace tutorials {
namespace graphics {

class Camera {
public:

	virtual const D3DXMATRIX& viewMatrix() const = 0;

	virtual const D3DXMATRIX& projectionMatrix() const = 0;

	D3DXVECTOR3 position() const;

protected:

	virtual ~Camera() {
	}

};

} // namespace graphics
} // namespace tutorials

#endif /* _GRAPHICS_CAMERA_HPP_ */
