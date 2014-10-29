#include "Actor.hpp"

#include "graphics/model_loaders/ObjModelLoader.hpp"

using namespace tutorials;
using namespace tutorials::entities;

Actor::Actor(graphics::Renderer* renderer) {
	graphics::model_loaders::ModelData data;
	graphics::model_loaders::ObjModelLoader loader;
	// loader.load(&data, "data/models/Daniel/craig chemise bleu/craig chemis bleu.obj");
	loader.load(&data, "data/models/cube.model");

	model_.initialise(&renderer->device(), data);

	renderer->addWorldModel(&model_);

	model_.moveTo(0.0f, 0.0f, 0.0f);
}

void Actor::update(const utils::Timer::Duration& duration) {
	float us = static_cast<float>(duration.total_microseconds());
	us *= 0.3f; // scale down

	model_.moveBy(0.0f, 0.0f, 0.000001f * us);
	model_.rotateBy(0.000003f * us, 0.0000001f * us, 0.000001f * us);
}
