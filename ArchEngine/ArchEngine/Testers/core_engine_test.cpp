/*===========================================================================*
 * Arch Engine - "Testers/core_engine_test.cpp"                              *
 *                                                                           *
 * This is a tester file. It's purpose is to test some functionalities, thus *
 * should be removed from releases.                                          *
 * Expect some hard coded, bizarre stuff. You've been warned ;)              *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 25/04/2018                                                       *
 * Last Modified: 01/05/2018                                                 *
 *===========================================================================*/


#include "../Config/engineMacros.hpp"

// Check if this tester is active
#if defined(ARCH_ENGINE_CORE_ENGINE_TEST)

#include "../Core/engine.hpp"

#include <cstdlib>


using namespace Core;


int main(int argc, char* argv[]) {
	if (Engine::getInstance().initialize("../../ArchEngine/Testers/"
		"core_engine_test_engine_config.lua")) {
		Engine::getInstance().run();
		Engine::getInstance().exit();
		return EXIT_SUCCESS;
	}

	return EXIT_FAILURE;
}

#endif	// ARCH_ENGINE_CORE_ENGINE_TEST