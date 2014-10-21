#include "Input.hpp"

#include <stdexcept>

using namespace tutorials;
using namespace tutorials::input;

void Input::initialise(const Properties& properties) {
	if (FAILED(
			DirectInput8Create(
				properties.systemContext.hInstance,
				DIRECTINPUT_VERSION,
				IID_IDirectInput8,
				reinterpret_cast<void**>(&directInput_.get()),
				0
				)
		)) {
		throw std::runtime_error("Failed to create a direct input interface");
	}
}

void Input::shutdown() {
	directInput_.reset();
}
