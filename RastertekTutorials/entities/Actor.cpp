#include "Actor.hpp"

#include "graphics/model_loaders/SimpleModelLoader.hpp"

using namespace tutorials;
using namespace tutorials::entities;

Actor::Actor(graphics::Renderer* renderer) {
	graphics::model_loaders::ModelData data;
	graphics::model_loaders::SimpleModelLoader loader;
	loader.load(&data, "data/models/jola.model");

	model_.initialise(&renderer->device(), data);

	renderer->addModel(&model_);

	model_.moveTo(0.0f, 0.0f, 0.1f);
}

void Actor::update(const utils::Timer::Duration& duration) {
	model_.moveBy(0.0f, 0.0f, 0.000001f * duration.total_microseconds());
	model_.rotateBy(0.000003f * duration.total_microseconds(), 0.0000001f * duration.total_microseconds(), 0.000001f * duration.total_microseconds());
}
