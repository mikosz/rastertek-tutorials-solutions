#include "ShaderConstantsBuffer.hpp"

#include <stdexcept>

using namespace tutorials;
using namespace tutorials::graphics;

void ShaderConstantsBuffer::initialise(ID3D11Device* device, size_t size, UINT miscFlags) {
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));

	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.ByteWidth = size;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = miscFlags;
	desc.StructureByteStride = 0;

	Buffer::initialise(device, desc);
}

void ShaderConstantsBuffer::bind(ID3D11DeviceContext* deviceContext, size_t slot) {
	ID3D11Buffer* buf = buffer();

	deviceContext->VSSetConstantBuffers(slot, 1, &buf);
}

void ShaderConstantsBuffer::write(ID3D11DeviceContext* deviceContext, const void* data, size_t size) {
	assert(size <= size_);

	D3D11_MAPPED_SUBRESOURCE mapped;

	if (FAILED(deviceContext->Map(buffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped))) {
		throw std::runtime_error("Failed to map resource");
	}

	std::memcpy(mapped.pData, data, size);

	deviceContext->Unmap(buffer(), 0);
}
