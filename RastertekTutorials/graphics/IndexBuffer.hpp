#ifndef _GRAPHICS_INDEXBUFFER_HPP_
#define _GRAPHICS_INDEXBUFFER_HPP_

#include <boost/cstdint.hpp>

#include "Buffer.hpp"

namespace tutorials {
namespace graphics {

class IndexBuffer : private Buffer {
public:

	void initialise(ID3D11Device* device, const boost::uint16_t* data, size_t indexCount, UINT miscFlags = 0);

	void initialise(ID3D11Device* device, const boost::uint32_t* data, size_t indexCount, UINT miscFlags = 0);

	void reset() {
		indexCount_ = 0;
		indexSize_ = 0;
		Buffer::reset();
	}

	void bind(ID3D11DeviceContext* deviceContext);

	size_t indexCount() const {
		return indexCount_;
	}

private:

	size_t indexCount_;

	size_t indexSize_;

	void doInitialise(ID3D11Device* device, const void* data, UINT miscFlags = 0);

};

} // namespace graphics
} // namespace tutorials

#endif /* _GRAPHICS_INDEXBUFFER_HPP_ */
