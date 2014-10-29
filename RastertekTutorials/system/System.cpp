#include "System.hpp"

#include "entities/Actor.hpp"
#include "entities/Sprite.hpp"
#include "graphics/RenderTargetTexture.hpp"

#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

using namespace tutorials;
using namespace tutorials::system;

System::System(const SystemContext& systemContext) :
	systemContext_(systemContext),
	applicationWindow_(systemContext),
	close_(false)
{
}

System::~System() {
	shutdown();
}

void System::initialise() {
	{
		Window::Properties properties;
		properties.width = 800;
		properties.height = 600;
		properties.title = "Rastertek Tutorials";
		properties.className = "RastertekTutorialsWindowClass";
		properties.parentMessageHandler = boost::bind(&System::messageCallback, this, _1, _2, _3, _4);

		applicationWindow_.open(properties);
	}

	{
		graphics::Device::Properties properties;
		properties.displayWidth = 800;
		properties.displayHeight = 600;
		properties.fullscreen = false;
		properties.vsync = false;
		properties.windowHandle = applicationWindow_.handle();

		renderer_.initialise(properties);
	}

	audio_.initialise(applicationWindow_.handle());
	// backgroundMusic_ = audio_.loadWaveFile("data/audio/HugeWAV.wav");

	{
		input::Input::Properties properties;
		properties.systemContext = systemContext_;

		input_.initialise(properties);
	}

	keyboard_.initialise(&input_, applicationWindow_.handle());
}

void System::shutdown() {
	keyboard_.shutdown();
	input_.shutdown();
	backgroundMusic_.reset();
	audio_.shutdown();
	renderer_.shutdown();
	applicationWindow_.close();
}

int System::run() {
	entities::Actor actor(&renderer_);

	graphics::RenderTargetTexture renderTargetTexture;
	renderTargetTexture.initialise(renderer_.device().d3dDevice(), 800, 600);
	renderTargetTexture.clear(renderer_.device().d3dDeviceContext(), renderer_.device().depthStencilView());
	entities::Sprite sprite(&renderer_, renderTargetTexture.shaderResourceView());

	// audio_.play(backgroundMusic_);

	boost::posix_time::ptime lastFrameEnd = boost::posix_time::microsec_clock::universal_time();
	boost::posix_time::time_duration lastFrameDuration;

	while (!close_) {
		MSG message;
		if (PeekMessage(&message, 0, 0, 0, PM_REMOVE)) {
			TranslateMessage(&message);
			DispatchMessage(&message);

			if (message.message == WM_QUIT) {
				close_ = true;
			}

			continue;
		}

		keyboard_.update();

		if (keyboard_.isPressed(DIK_ESCAPE)) {
			close_ = true;
			break;
		}

		actor.update(lastFrameDuration);

		{
			renderTargetTexture.clear(renderer_.device().d3dDeviceContext(), renderer_.device().depthStencilView());
			renderTargetTexture.use(renderer_.device().d3dDeviceContext(), renderer_.device().depthStencilView());

			graphics::A3DCamera oldCam = renderer_.worldCamera();
			
			{
				graphics::A3DCamera::Properties newProps;
				newProps.aspectRatio = 800.0f / 600.0f;
				newProps.nearPlane = 0.1f;
				newProps.farPlane = 1000.0f;
				newProps.fieldOfView = static_cast<float>(D3DXToRadian(40.0f));
				newProps.lookAt = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				newProps.position = D3DXVECTOR3(0.0f, 0.0f, 7.0f);
				newProps.up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
				renderer_.worldCamera().initialise(newProps);
			}

			renderer_.renderWorld();

			renderer_.worldCamera() = oldCam;
		}

		renderer_.beginScene();
		renderer_.renderWorld();
		renderer_.renderHUD();
		renderer_.endScene();

		boost::posix_time::ptime now = boost::posix_time::microsec_clock::universal_time();
		lastFrameDuration = now - lastFrameEnd;
		lastFrameEnd = now;
	}

	return 0;
}

LRESULT System::messageCallback(HWND window, UINT message, WPARAM wparam, LPARAM lparam) {
	return DefWindowProc(window, message, wparam, lparam);
}
