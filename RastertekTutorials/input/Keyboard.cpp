#include "Keyboard.hpp"

#include <stdexcept>

#include "Input.hpp"

using namespace tutorials;
using namespace tutorials::input;

void Keyboard::initialise(Input* input, system::Window::Handle windowHandle) {
	if (FAILED(input->context()->CreateDevice(GUID_SysKeyboard, &device_.get(), 0))) {
		throw std::runtime_error("Failed to create a keyboard device");
	}

	if (FAILED(device_->SetDataFormat(&c_dfDIKeyboard))) {
		throw std::runtime_error("Failed to set keyboard data format");
	}

	if (FAILED(device_->SetCooperativeLevel(windowHandle, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))) {
		throw std::runtime_error("Failed to set cooperative level for keyboard");
	}

	device_->Acquire();
}

void Keyboard::shutdown() {
	if (device_) {
		device_->Unacquire();
		device_.reset();
	}
}

void Keyboard::update() {
	for (;;) {
		HRESULT result = device_->GetDeviceState(sizeof(keyboardState_), keyboardState_);
		if (SUCCEEDED(result)) {
			break;
		} else if (result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED) {
			if (FAILED(device_->Acquire())) {
				std::memset(keyboardState_, 0, sizeof(keyboardState_));
				break;
			}
		} else {
			throw std::runtime_error("Failed to get keyboard state");
		}
	}
}
