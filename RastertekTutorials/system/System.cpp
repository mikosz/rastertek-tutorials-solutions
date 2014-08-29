#include "System.hpp"

#include "entities/Actor.hpp"

#include <boost/bind.hpp>

using namespace tutorials;
using namespace tutorials::system;

System::System(const SystemContext& systemContext) :
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
}

void System::shutdown() {
	renderer_.shutdown();
	applicationWindow_.close();
}

int System::run() {
	entities::Actor actor(&renderer_);

	graphics::Camera::Properties properties;
	properties.aspectRatio = 800.0f / 600.0f;
	properties.nearPlane = 1.0f;
	properties.farPlane = 1000.0f;
	properties.fieldOfView = 60.0f;
	properties.lookAt = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	properties.position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	properties.up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	graphics::Camera camera(properties);

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

		renderer_.renderFrame(camera);
	}

	return 0;
}

LRESULT System::messageCallback(HWND window, UINT message, WPARAM wparam, LPARAM lparam) {
	return DefWindowProc(window, message, wparam, lparam);
}
