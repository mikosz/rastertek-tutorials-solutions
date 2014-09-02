#ifndef _GRAPHICS_MODELLOADERS_SIMPLEMODELLOADER_HPP_
#define _GRAPHICS_MODELLOADERS_SIMPLEMODELLOADER_HPP_

#include "ModelLoader.hpp"

namespace tutorials {
namespace graphics {
namespace model_loaders {

class ModelData;

class SimpleModelLoader : public ModelLoader {
public:

	void load(ModelData* modelData, const boost::filesystem::path& sourcePath);

};

} // namespace model_loaders
} // namespace graphics
} // namespace tutorials

#endif /* _GRAPHICS_MODELLOADERS_SIMPLEMODELLOADER_HPP_ */
