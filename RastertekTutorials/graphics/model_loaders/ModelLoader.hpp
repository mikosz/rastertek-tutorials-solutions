#ifndef _GRAPHICS_MODELLOADERS_MODELLOADER_HPP_
#define _GRAPHICS_MODELLOADERS_MODELLOADER_HPP_

#include <boost/filesystem/path.hpp>

namespace tutorials {
namespace graphics {
namespace model_loaders {

class ModelData;

class ModelLoader {
public:

	virtual void load(ModelData* modelData, const boost::filesystem::path& sourcePath) = 0;

protected:

	virtual ~ModelLoader() {
	}

};

} // namespace model_loaders
} // namespace graphics
} // namespace tutorials

#endif /* _GRAPHICS_MODELLOADERS_MODELLOADER_HPP_ */
