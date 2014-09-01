#include "Model.hpp"

#include <fstream>
#include <iterator>

#include <boost/spirit/include/qi.hpp>

#include "Device.hpp"
#include "Camera.hpp"

using namespace tutorials;
using namespace tutorials::graphics;

void Model::Data::loadFromModelFile(const boost::filesystem::path& path, Data* modelData) {
	std::ifstream ifs(path.string().c_str());
	using boost::spirit::qi::float_;
	using boost::spirit::qi::repeat;
	using boost::spirit::ascii::space;

	std::string line;
	while (getline(ifs, line)) {
		std::string::iterator it = line.begin(), end = line.end();

		bool result = boost::spirit::qi::phrase_parse(
			it, end,
			(
				float_ >> float_ >> float_ >> float_ >> float_ >> float_ >> float_ >> float_
			),
			space);

		if (it != end) {
			throw std::runtime_error("Failed to load module from " + path.string());
		}
	}
}

void Model::initialise(Device* device, const Data& data) {
	assert(!data.vertices.empty());
	assert(!data.indices.empty());

	D3DXMatrixIdentity(&worldMatrix_);

	vertexBuffer_.initialise(
		device->d3dDevice(),
		&data.vertices.front(),
		data.vertices.size(),
		sizeof(VertexShader::Vertex)
		);

	indexBuffer_.initialise(
		device->d3dDevice(),
		&data.indices.front(),
		data.indices.size(),
		data.topology
		);

	texture_.initialise(device->d3dDevice(), data.texturePath);
}

void Model::reset() {
	vertexBuffer_.reset();
	indexBuffer_.reset();
	texture_.reset();
}

void Model::render(Device* device, const Camera& camera) {
	vertexBuffer_.bind(device->d3dDeviceContext(), 0);
	indexBuffer_.bind(device->d3dDeviceContext());

	device->d3dDeviceContext()->DrawIndexed(indexBuffer_.indexCount(), 0, 0);
}
