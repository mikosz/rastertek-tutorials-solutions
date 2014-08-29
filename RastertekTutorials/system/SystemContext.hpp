#ifndef _SYSTEM_SYSTEM_CONTEXT_HPP_
#define _SYSTEM_SYSTEM_CONTEXT_HPP_

#define WIN32_LEAN_AND_MEAN

#include <windows.h>

namespace tutorials {
namespace system {

struct SystemContext {

	HINSTANCE hInstance;

	HINSTANCE hPrevInstance;

	PSTR pScmdline;

	int iCmdshow;

	SystemContext(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow) :
		hInstance(hInstance),
		hPrevInstance(hPrevInstance),
		pScmdline(pScmdline),
		iCmdshow(iCmdshow)
	{
	}

};

} // namespace system
} // namespace tutorials

#endif /* _SYSTEM_SYSTEM_CONTEXT_HPP_ */
