#ifndef _GRAPHICS_VERTEXBUFFER_HPP_
#define _GRAPHICS_VERTEXBUFFER_HPP_

#include "Buffer.hpp"

namespace tutorials {
namespace graphics {

class VertexBuffer : private Buffer {
public:

	void initialise(ID3D11Device* device, const void* data, size_t vertexCount, size_t vertexSize, UINT miscFlags = 0);

	void reset() {
		vertexSize_ = 0;
		Buffer::reset();
	}

	void bind(ID3D11DeviceContext* deviceContext, size_t slot);

private:

	size_t vertexSize_;

};

} // namespace graphics
} // namespace tutorials

#endif /* _GRAPHICS_VERTEXBUFFER_HPP_ */
