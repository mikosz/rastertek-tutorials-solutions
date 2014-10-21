#ifndef _INPUT_INPUT_HPP_
#define _INPUT_INPUT_HPP_

#define DIRECTINPUT_VERSION 0x0800
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
#include <dinput.h>

#include "system/SystemContext.hpp"
#include "utils/COMWrapper.hpp"

namespace tutorials {
namespace input {

class Input {
public:

	struct Properties {

		system::SystemContext systemContext;

	};

	~Input() {
		shutdown();
	}

	void initialise(const Properties& properties);

	void shutdown();

	IDirectInput8* context() {
		return directInput_.get();
	}

private:

	utils::COMWrapper<IDirectInput8> directInput_;

};

} // namespace input
} // namespace tutorials

#endif /* _INPUT_INPUT_HPP_ */
