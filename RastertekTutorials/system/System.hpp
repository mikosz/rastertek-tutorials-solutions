#ifndef _SYSTEM_SYSTEM_HPP_
#define _SYSTEM_SYSTEM_HPP_

#define WIN32_LEAN_AND_MEAN

#include <windows.h>

#include "Window.hpp"
#include "SystemContext.hpp"
#include "graphics/Renderer.hpp"

namespace tutorials {
namespace system {

class System {
public:

	System(const SystemContext& systemContext);

	~System();

	void initialise();

	void shutdown();

	int run();

private:

	Window applicationWindow_;

	graphics::Renderer renderer_;

	bool close_;

	LRESULT messageCallback(HWND window, UINT message, WPARAM wparam, LPARAM lparam);

};

} // namespace system
} // namespace tutorials

#endif /* _SYSTEM_SYSTEM_HPP_ */
