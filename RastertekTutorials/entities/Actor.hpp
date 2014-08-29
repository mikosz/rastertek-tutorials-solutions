#ifndef _ENTITIES_ACTOR_HPP_
#define _ENTITIES_ACTOR_HPP_

#include "graphics/Model.hpp"
#include "graphics/Renderer.hpp"
#include "utils/Timer.hpp"

namespace tutorials {
namespace entities {

class Actor {
public:

	Actor(graphics::Renderer* renderer);

	void update(const utils::Timer::Duration& duration);

private:

	graphics::Model model_;

	float x, y, z;

};

} // namespace entities
} // namespace tutorials

#endif /* _ENTITIES_ACTOR_HPP_ */
