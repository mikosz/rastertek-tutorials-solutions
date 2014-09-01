#ifndef _GRAPHICS_SHADERCONSTANTSBUFFER_HPP_
#define _GRAPHICS_SHADERCONSTANTSBUFFER_HPP_

#include "Buffer.hpp"

namespace tutorials {
namespace graphics {

class ShaderConstantsBuffer : private Buffer {
public:

	enum ShaderType {
		PIXEL,
		VERTEX,
	};

	void initialise(ID3D11Device* device, size_t size, UINT miscFlags = 0);

	void reset() {
		size_ = 0;

		Buffer::reset();
	}

	void bind(ID3D11DeviceContext* deviceContext, ShaderType shaderType, size_t slot);

	void write(ID3D11DeviceContext* deviceContext, const void* data, size_t size);

private:

	size_t size_;

};

} // namespace graphics
} // namespace tutorials

#endif /* _GRAPHICS_SHADERCONSTANTSBUFFER_HPP_ */
