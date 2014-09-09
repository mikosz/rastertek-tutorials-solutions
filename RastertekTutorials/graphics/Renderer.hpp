#ifndef _GRAPHICS_RENDERER_HPP_
#define _GRAPHICS_RENDERER_HPP_

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")

#include <list>

#include "Device.hpp"
#include "Model.hpp"
#include "Camera.hpp"
#include "shaders/LightingVertexShader.hpp"
#include "shaders/LightingPixelShader.hpp"
#include "shaders/SpritePixelShader.hpp"
#include "shaders/SpriteVertexShader.hpp"

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

	shaders::LightingVertexShader worldVertexShader_;

	shaders::LightingPixelShader worldPixelShader_;

	shaders::SpriteVertexShader hudVertexShader_;

	shaders::SpritePixelShader hudPixelShader_;

};

} // namespace graphics
} // namespace tutorials

#endif /* _GRAPHICS_RENDERER_HPP_ */
