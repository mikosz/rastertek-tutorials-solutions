#include "System.hpp"

#include "entities/Actor.hpp"

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
		renderer_.renderFrame();

		boost::posix_time::ptime now = boost::posix_time::microsec_clock::universal_time();
		lastFrameDuration = now - lastFrameEnd;
		lastFrameEnd = now;
	}

	return 0;
}

LRESULT System::messageCallback(HWND window, UINT message, WPARAM wparam, LPARAM lparam) {
	return DefWindowProc(window, message, wparam, lparam);
}
