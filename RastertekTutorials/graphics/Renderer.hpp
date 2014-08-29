#ifndef _GRAPHICS_RENDERER_HPP_
#define _GRAPHICS_RENDERER_HPP_

#include <list>

#include "Device.hpp"
#include "Model.hpp"
#include "Camera.hpp"
#include "VertexShader.hpp"
#include "PixelShader.hpp"

namespace tutorials {
namespace graphics {

class Renderer {
public:

	~Renderer() {
		shutdown();
	}

	void initialise(const Device::Properties& properties);

	void shutdown();

	void renderFrame(const Camera& camera);

	void addModel(Model* model) {
		pipeline_.push_back(model);
	}

	Device& device() {
		return device_;
	}

private:

	Device device_;

	std::list<Model*> pipeline_;

	VertexShader vertexShader_;

	PixelShader pixelShader_;

};

} // namespace graphics
} // namespace tutorials

#endif /* _GRAPHICS_RENDERER_HPP_ */
