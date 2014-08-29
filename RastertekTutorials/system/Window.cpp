#include "Window.hpp"

#include <sstream>

using namespace tutorials;
using namespace tutorials::system;

Window::Window(const SystemContext& systemContext) :
	systemContext_(systemContext),
	handle_(0)
{
}

Window::~Window() {
	close();
}

void Window::open(const Properties& properties) {
	close();

	currentProperties_ = properties;

	WNDCLASSEX wndClassEx;
	std::memset(&wndClassEx, 0, sizeof(WNDCLASSEX));

	wndClassEx.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wndClassEx.lpfnWndProc = &messageHandler;
	wndClassEx.hInstance = systemContext_.hInstance;
	wndClassEx.hIcon = LoadIcon(0, IDI_WINLOGO);
	wndClassEx.hIconSm = wndClassEx.hIcon;
	wndClassEx.hCursor = LoadCursor(0, IDC_ARROW);
	wndClassEx.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
	wndClassEx.lpszClassName = properties.className.c_str();
	wndClassEx.cbSize = sizeof(WNDCLASSEX);

	if (RegisterClassEx(&wndClassEx) == 0) {
		std::ostringstream error;
		error << "Failed to register window class. Error code: " << GetLastError();

		throw std::runtime_error(error.str());
	}

	handle_ = CreateWindowEx(
		WS_EX_APPWINDOW,
		properties.className.c_str(),
		properties.title.c_str(),
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		100,
		100,
		properties.width,
		properties.height,
		0,
		0,
		systemContext_.hInstance,
		this
		);

	if (handle_ == 0) {
		std::ostringstream error;
		error << "Failed to create window. Error code: " << GetLastError();

		throw std::runtime_error(error.str());
	}

	ShowWindow(handle_, SW_SHOW);
	SetForegroundWindow(handle_);
	SetFocus(handle_);

	ShowCursor(FALSE);
}

void Window::close() {
	if (handle_ != 0) {
		ShowCursor(TRUE);

		DestroyWindow(handle_);
		handle_ = 0;

		UnregisterClass(currentProperties_.className.c_str(), systemContext_.hInstance);
	}
}

LRESULT CALLBACK Window::messageHandler(HWND window, UINT message, WPARAM wparam, LPARAM lparam) {
	switch (message)
	{
	case WM_NCCREATE: {
		LPCREATESTRUCT createStruct = reinterpret_cast<LPCREATESTRUCT>(lparam);
		Window* instance = reinterpret_cast<Window*>(createStruct->lpCreateParams);

		if (!instance) {
			throw std::logic_error("Window instance create parameter shall not be null!");
		}

		::SetWindowLong(window, GWL_USERDATA, reinterpret_cast<LONG>(instance));

		return TRUE;
	}
	case WM_CLOSE:
	case WM_DESTROY:
		PostQuitMessage(0);
		return FALSE;
	default: {
		Window* instance = reinterpret_cast<Window*>(::GetWindowLong(window, GWL_USERDATA));

		if (!instance) {
			throw std::logic_error("Window instance shall not be null!");
		}

		if (instance->currentProperties_.parentMessageHandler) {
			return instance->currentProperties_.parentMessageHandler(window, message, wparam, lparam);
		} else {
			return DefWindowProc(window, message, wparam, lparam);
		}
	}
	}
}
