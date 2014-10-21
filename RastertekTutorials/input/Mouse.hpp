#ifndef _INPUT_MOUSE_HPP_
#define _INPUT_MOUSE_HPP_

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#include "system/Window.hpp"
#include "utils/COMWrapper.hpp"

namespace tutorials {
namespace input {

class Input;

class Mouse {
public:

	void initialise(Input* input, system::Window::Handle windowHandle);

	void shutdown();

	void update();

private:

	utils::COMWrapper<IDirectInputDevice8> device_;

	DIMOUSESTATE mouseState_;

};

} // namespace input
} // namespace tutorials

#endif /* _INPUT_MOUSE_HPP_ */
