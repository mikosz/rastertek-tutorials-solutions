#ifndef _GRAPHICS_SPRITE_HPP_
#define _GRAPHICS_SPRITE_HPP_

#include <boost/filesystem/path.hpp>

#include "Texture.hpp"

namespace tutorials {
namespace graphics {

class Sprite {
public:

	void initialise(const boost::filesystem::path& texturePath);

	void shutdown();

private:

	Texture texture_;

};

} // namespace graphics
} // namespace tutorials

#endif /* _GRAPHICS_SPRITE_HPP_ */
