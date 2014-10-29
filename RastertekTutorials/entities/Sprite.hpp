#ifndef _ENTITIES_SPRITE_HPP_
#define _ENTITIES_SPRITE_HPP_

#include "graphics/Renderer.hpp"
#include "graphics/Texture.hpp"

namespace tutorials {
namespace entities {

class Sprite {
public:

	Sprite(graphics::Renderer* renderer, utils::COMWrapper<ID3D11ShaderResourceView> shaderResourceView);

private:

	graphics::Model model_;

	float x, y, z;

};

} // namespace entities
} // namespace tutorials

#endif /* _ENTITIES_SPRITE_HPP_ */
