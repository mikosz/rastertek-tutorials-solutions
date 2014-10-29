#include "Sprite.hpp"

using namespace tutorials;
using namespace tutorials::entities;

Sprite::Sprite(graphics::Renderer* renderer, utils::COMWrapper<ID3D11ShaderResourceView> shaderResourceView) {
	graphics::model_loaders::ModelData data;

	graphics::model_loaders::ModelData::Vertex vertex;

	data.addGroup(graphics::model_loaders::ModelData::TRIANGLE_LIST);

	vertex.position = D3DXVECTOR3(-0.5f, -0.5f, 0.0f);
	vertex.textureCoordinate = D3DXVECTOR2(0.0f, 1.0f);
	data.addVertex(vertex);

	vertex.position = D3DXVECTOR3(-0.5f, 0.5f, 0.0f);
	vertex.textureCoordinate = D3DXVECTOR2(0.0f, 0.0f);
	data.addVertex(vertex);

	vertex.position = D3DXVECTOR3(0.5f, -0.5f, 0.0f);
	vertex.textureCoordinate = D3DXVECTOR2(1.0f, 1.0f);
	data.addVertex(vertex);

	vertex.position = D3DXVECTOR3(-0.5f, 0.5f, 0.0f);
	vertex.textureCoordinate = D3DXVECTOR2(0.0f, 0.0f);
	data.addVertex(vertex);

	vertex.position = D3DXVECTOR3(0.5f, 0.5f, 0.0f);
	vertex.textureCoordinate = D3DXVECTOR2(1.0f, 0.0f);
	data.addVertex(vertex);

	vertex.position = D3DXVECTOR3(0.5f, -0.5f, 0.0f);
	vertex.textureCoordinate = D3DXVECTOR2(1.0f, 1.0f);
	data.addVertex(vertex);

	data.addIndex(0);
	data.addIndex(1);
	data.addIndex(2);
	data.addIndex(3);
	data.addIndex(4);
	data.addIndex(5);

	model_.initialise(&renderer->device(), data);
	model_.baseTexture().initialise(shaderResourceView);

	renderer->addHudModel(&model_);

	model_.moveTo(0.1f, 0.1f, 0.5f);
	model_.scale(0.2f, 0.2f, 1.0f);
}
