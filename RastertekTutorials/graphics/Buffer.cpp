#include "Buffer.hpp"

#include "Device.hpp"

using namespace tutorials;
using namespace tutorials::graphics;

void Buffer::initialise(ID3D11Device* device, const D3D11_BUFFER_DESC& desc) {
	if (FAILED(device->CreateBuffer(&desc, 0, &buffer_.get()))) {
		throw std::runtime_error("Failed to create a buffer");
	}
}

void Buffer::initialise(ID3D11Device* device, const D3D11_BUFFER_DESC& desc, const D3D11_SUBRESOURCE_DATA& data) {
	if (FAILED(device->CreateBuffer(&desc, &data, &buffer_.get()))) {
		throw std::runtime_error("Failed to create a buffer");
	}
}
