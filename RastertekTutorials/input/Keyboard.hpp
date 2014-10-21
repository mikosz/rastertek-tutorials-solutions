#ifndef _INPUT_KEYBOARD_HPP_
#define _INPUT_KEYBOARD_HPP_

#include <cstdint>

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#include "system/Window.hpp"
#include "utils/COMWrapper.hpp"

namespace tutorials {
namespace input {

class Input;

class Keyboard {
public:

	~Keyboard() {
		shutdown();
	}

	void initialise(Input* input, system::Window::Handle windowHandle);

	void shutdown();

	void update();

	bool isPressed(size_t key) {
		return (keyboardState_[key] & 0x80) != 0;
	}

private:

	utils::COMWrapper<IDirectInputDevice8> device_;

	std::uint8_t keyboardState_[256];

};

} // namespace input
} // namespace tutorials

#endif /* _INPUT_KEYBOARD_HPP_ */
