#include "Mouse.hpp"

#include <stdexcept>

#include "Input.hpp"

using namespace tutorials;
using namespace tutorials::input;

void Mouse::initialise(Input* input, system::Window::Handle windowHandle) {
	if (FAILED(input->context()->CreateDevice(GUID_SysMouse, &device_.get(), 0))) {
		throw std::runtime_error("Failed to create a mouse device");
	}

	if (FAILED(device_->SetDataFormat(&c_dfDIMouse))) {
		throw std::runtime_error("Failed to set mouse data format");
	}

	if (FAILED(device_->SetCooperativeLevel(windowHandle, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))) {
		throw std::runtime_error("Failed to set cooperative level for mouse");
	}

	if (FAILED(device_->Acquire())) {
		throw std::runtime_error("Failed to acquire the mouse device");
	}
}

void Mouse::shutdown() {
	device_->Unacquire();
	device_.reset();
}

void Mouse::update() {
	for (;;) {
		HRESULT result = device_->GetDeviceState(sizeof(mouseState_), &mouseState_);
		if (SUCCEEDED(result)) {
			break;
		}
		else if (result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED) {
			if (FAILED(device_->Acquire())) {
				throw std::runtime_error("Failed to re-acquire the mouse device");
			}
		}
		else {
			throw std::runtime_error("Failed to get mouse state");
		}
	}
}
