#ifndef _SYSTEM_WINDOW_HPP_
#define _SYSTEM_WINDOW_HPP_

#include <string>

#include <boost/function.hpp>

#include "SystemContext.hpp"

namespace tutorials {
namespace system {

class Window {
public:

	typedef boost::function<LRESULT (HWND, UINT, WPARAM, LPARAM)> MessageHandler;

	typedef HWND Handle;

	struct Properties {

		size_t width;

		size_t height;

		std::string title;

		std::string className;

		MessageHandler parentMessageHandler;

	};

	Window(const SystemContext& systemContext);

	~Window();

	void open(const Properties& properties);

	void close();

	Handle handle() {
		return handle_;
	}

private:

	SystemContext systemContext_;

	Properties currentProperties_;

	HWND handle_;

	static LRESULT CALLBACK messageHandler(HWND window, UINT message, WPARAM wparam, LPARAM lparam);

};

} // namespace system
} // namespace tutorials

#endif /* _SYSTEM_WINDOW_HPP_ */
