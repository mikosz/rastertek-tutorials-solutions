#include "Actor.hpp"

using namespace tutorials;
using namespace tutorials::entities;

Actor::Actor(graphics::Renderer* renderer) {
	graphics::Model::Data data;
	graphics::Model::Data::loadFromModelFile("data/models/jola.model", &data);

	model_.initialise(&renderer->device(), data);

	renderer->addModel(&model_);

	model_.moveTo(0.0f, 0.0f, 0.1f);
}

void Actor::update(const utils::Timer::Duration& duration) {
	model_.moveBy(0.0f, 0.0f, 0.000001f * duration.total_microseconds());
	model_.rotateBy(0.000003f * duration.total_microseconds(), 0.0000001f * duration.total_microseconds(), 0.000001f * duration.total_microseconds());
}
