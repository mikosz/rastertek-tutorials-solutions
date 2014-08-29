#include "system/System.hpp"

#include <sstream>

using namespace tutorials;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow) {
	try {
		system::SystemContext context(hInstance, hPrevInstance, pScmdline, iCmdshow);
		system::System systemInstance(context);
		systemInstance.initialise();
		int result = systemInstance.run();
		systemInstance.shutdown();
		return result;
	} catch (const std::exception& e) {
		std::ostringstream error;
		error << "Caught exception: " << e.what() << '\n';

		MessageBox(0, error.str().c_str(), "Error", 0);

		return 1;
	}

	return 0;
}
