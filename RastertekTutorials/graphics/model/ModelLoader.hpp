#ifndef _GRAPHICS_MODEL_MODELLOADER_HPP_
#define _GRAPHICS_MODEL_MODELLOADER_HPP_

#include <boost/filesystem/path.hpp>

namespace tutorials {
namespace graphics {
namespace model {

class ModelLoader {
public:

	virtual void load(ModelData* modelData, boost::filesystem::path& sourcePath) = 0;

protected:

	virtual ~ModelLoader() {
	}

};

} // namespace model
} // namespace graphics
} // namespace tutorials

#endif /* _GRAPHICS_MODEL_MODELLOADER_HPP_ */
