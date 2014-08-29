#ifndef _GRAPHICS_BUFFER_HPP_
#define _GRAPHICS_BUFFER_HPP_

#include <d3d11.h>

#include "utils/COMWrapper.hpp"

namespace tutorials {
namespace graphics {

class Device;

class Buffer {
protected:

	~Buffer() {
	}

	void initialise(ID3D11Device* device, const D3D11_BUFFER_DESC& desc);

	void initialise(ID3D11Device* device, const D3D11_BUFFER_DESC& desc, const D3D11_SUBRESOURCE_DATA& data);

	void reset() {
		buffer_.reset();
	}

	ID3D11Buffer* buffer() {
		return buffer_;
	}

private:

	utils::COMWrapper<ID3D11Buffer> buffer_;

};

} // namespace graphics
} // namespace tutorials

#endif /* _GRAPHICS_BUFFER_HPP_ */
