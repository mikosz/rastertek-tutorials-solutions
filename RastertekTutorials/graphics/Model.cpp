#include "Model.hpp"

#include "Device.hpp"
#include "Camera.hpp"

using namespace tutorials;
using namespace tutorials::graphics;

Model::Model(Device* device, const Data& data) {
	assert(!data.vertices.empty());
	assert(!data.indices.empty());

	vertexBuffer_.initialise(
		device->d3dDevice(),
		&data.vertices.front(),
		data.vertices.size(),
		sizeof(VertexShader::Vertex)
		);
	indexBuffer_.initialise(
		device->d3dDevice(),
		&data.indices.front(),
		data.indices.size()
		);
}

void Model::render(Device* device, const Camera& camera) {
	vertexBuffer_.bind(device->d3dDeviceContext(), 0);
	indexBuffer_.bind(device->d3dDeviceContext());

	device->d3dDeviceContext()->DrawIndexed(indexBuffer_.indexCount(), 0, 0);
}
