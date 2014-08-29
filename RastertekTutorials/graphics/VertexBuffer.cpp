#include "VertexBuffer.hpp"

using namespace tutorials;
using namespace tutorials::graphics;

void VertexBuffer::initialise(ID3D11Device* device, const void* data, size_t vertexCount, size_t vertexSize, UINT miscFlags) {
	vertexSize_ = vertexSize;

	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));

	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.ByteWidth = vertexCount * vertexSize;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = miscFlags;
	desc.StructureByteStride = 0;
	desc.Usage = D3D11_USAGE_IMMUTABLE;

	D3D11_SUBRESOURCE_DATA subresourceData;
	ZeroMemory(&subresourceData, sizeof(subresourceData));

	subresourceData.pSysMem = data;

	Buffer::initialise(device, desc, subresourceData);
}

void VertexBuffer::bind(ID3D11DeviceContext* deviceContext, size_t slot) {
	UINT stride = vertexSize_;
	UINT offset = 0;
	ID3D11Buffer* buf = buffer();

	deviceContext->IASetVertexBuffers(slot, 1, &buf, &stride, &offset);
}
