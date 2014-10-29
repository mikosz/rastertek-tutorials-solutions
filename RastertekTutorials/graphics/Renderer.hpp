#ifndef _GRAPHICS_RENDERER_HPP_
#define _GRAPHICS_RENDERER_HPP_

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")

#include <list>

#include "Device.hpp"
#include "Model.hpp"
#include "A3DCamera.hpp"
#include "A2DCamera.hpp"
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

	void beginScene();

	void endScene();

	void renderWorld();

	void renderHUD();

	void addWorldModel(Model* model) {
		worldPipeline_.push_back(model);
	}

	void addHudModel(Model* model) {
		hudPipeline_.push_back(model);
	}

	Device& device() {
		return device_;
	}

	A3DCamera& worldCamera() {
		return worldCamera_;
	}

private:

	Device device_;

	std::list<Model*> worldPipeline_;

	shaders::LightingVertexShader worldVertexShader_;

	shaders::LightingPixelShader worldPixelShader_;

	A3DCamera worldCamera_;

	std::list<Model*> hudPipeline_;

	shaders::SpriteVertexShader hudVertexShader_;

	shaders::SpritePixelShader hudPixelShader_;

	A2DCamera hudCamera_;

};

} // namespace graphics
} // namespace tutorials

#endif /* _GRAPHICS_RENDERER_HPP_ */
