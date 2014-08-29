#include "Actor.hpp"

using namespace tutorials;
using namespace tutorials::entities;

namespace /* anonymous */ {

const graphics::Model::Data& getModelData() {
	static graphics::Model::Data modelData;

	if (modelData.vertices.empty()) {
		{
			graphics::VertexShader::Vertex v;

			v.colour = D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f);

			v.position = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);
			modelData.vertices.push_back(v);

			v.position = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			modelData.vertices.push_back(v);

			v.position = D3DXVECTOR3(1.0f, -1.0f, 0.0f);
			modelData.vertices.push_back(v);
		}

		modelData.indices.push_back(0);
		modelData.indices.push_back(1);
		modelData.indices.push_back(2);
	}

	return modelData;
}

} // anonymous namespace

Actor::Actor(graphics::Renderer* renderer) :
	model_(&renderer->device(), getModelData())
{
	renderer->addModel(&model_);
	model_.moveTo(0.0f, 0.0f, 10.0f);
}

void Actor::update(const utils::Timer::Duration& duration) {

}
