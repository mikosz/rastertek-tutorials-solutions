#include "Model.hpp"

#include "Device.hpp"
#include "Camera.hpp"

using namespace tutorials;
using namespace tutorials::graphics;

namespace /* anonymous */ {

D3D_PRIMITIVE_TOPOLOGY toDXTopology(model_loaders::ModelData::Topology topology) {
	switch (topology) {
	case model_loaders::ModelData::TRIANGLE_LIST:
		return D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	case model_loaders::ModelData::TRIANGLE_STRIP:
		return D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
	default:
		throw std::logic_error("Invalid primitive topology");
	}
}

} // anonymous namespace

void Model::initialise(Device* device, const model_loaders::ModelData& data) {
	D3DXMatrixIdentity(&worldMatrix_);

	if (data.faces().empty() || data.vertices().empty() || data.indices().empty()) {
		throw std::runtime_error("Model cannot be empty");
	}

	vertexBuffer_.initialise(
		device->d3dDevice(),
		&data.vertices().front(),
		data.vertices().size(),
		sizeof(VertexShader::Vertex)
		);

	// TODO: change indices type to uint16 if possible here (depending on max index)
	indexBuffer_.initialise(
		device->d3dDevice(),
		&data.indices().front(),
		data.indices().size()
		);

	facesData_ = data.faces();

	texture_.initialise(device->d3dDevice(), "data/textures/jola.dds");
}

void Model::reset() {
	vertexBuffer_.reset();
	indexBuffer_.reset();
	facesData_.clear();
	texture_.reset();
}

void Model::render(Device* device, const Camera& camera) {
	vertexBuffer_.bind(device->d3dDeviceContext(), 0);
	indexBuffer_.bind(device->d3dDeviceContext());

	model_loaders::ModelData::Faces::iterator it, end = facesData_.end();
	for (it = facesData_.begin(); it != end; ++it) {
		device->d3dDeviceContext()->IASetPrimitiveTopology(toDXTopology(it->topology));

		size_t indexCount;
		model_loaders::ModelData::Faces::iterator next = it;
		++next;
		if (next == end) {
			indexCount = indexBuffer_.indexCount() - it->firstIndexIndex;
		}
		else {
			indexCount = next->firstIndexIndex - it->firstIndexIndex;
		}

		device->d3dDeviceContext()->DrawIndexed(indexCount, it->firstIndexIndex, 0);
	}
}