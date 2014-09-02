#include "IndexBuffer.hpp"

#include <cassert>

using namespace tutorials;
using namespace tutorials::graphics;

void IndexBuffer::initialise(
	ID3D11Device* device,
	const boost::uint16_t* data,
	size_t indexCount,
	UINT miscFlags
	) {
	indexSize_ = 2;
	indexCount_ = indexCount;

	doInitialise(device, data, miscFlags);
}

void IndexBuffer::initialise(
	ID3D11Device* device,
	const boost::uint32_t* data,
	size_t indexCount,
	UINT miscFlags
	) {
	indexSize_ = 4;
	indexCount_ = indexCount;

	doInitialise(device, data, miscFlags);
}

void IndexBuffer::bind(ID3D11DeviceContext* deviceContext) {
	assert(indexSize_ == 2 || indexSize_ == 4);

	DXGI_FORMAT indexFormat;
	if (indexSize_ == 2) {
		indexFormat = DXGI_FORMAT_R16_UINT;
	} else {
		indexFormat = DXGI_FORMAT_R32_UINT;
	}

	deviceContext->IASetIndexBuffer(buffer(), indexFormat, 0);
}

void IndexBuffer::doInitialise(ID3D11Device* device, const void* data, UINT miscFlags) {
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));

	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	desc.ByteWidth = indexCount_ * indexSize_;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = miscFlags;
	desc.StructureByteStride = 0;
	desc.Usage = D3D11_USAGE_IMMUTABLE;

	D3D11_SUBRESOURCE_DATA subresourceData;
	ZeroMemory(&subresourceData, sizeof(subresourceData));

	subresourceData.pSysMem = data;

	Buffer::initialise(device, desc, subresourceData);
}
