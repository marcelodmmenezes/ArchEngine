/*===========================================================================*
 * Arch Engine - "Config/engineMacros.hpp"                                   *
 *                                                                           *
 * Macros for engine compile configuration                                   *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 10/04/2018                                                       *
 * Last Modified: 01/05/2018                                                 *
 *===========================================================================*/


#ifndef CONFIG_ENGINE_MACROS_HPP
#define CONFIG_ENGINE_MACROS_HPP


// Testers
//#define ARCH_ENGINE_CORE_ENGINE_TEST
//#define ARCH_ENGINE_OS_INPUT_CONTEXT_TEST
#define ARCH_ENGINE_OS_WINDOW_TEST
//#define ARCH_ENGINE_ECS_ECS_TEST
//#define ARCH_ENGINE_SCRIPT_LUA_SCRIPT_TEST
//#define ARCH_ENGINE_UTILS_LOGGER_TEST

// Assertion macros
//#define ARCH_ENGINE_REMOVE_ASSERTIONS

// Logging macros
//#define ARCH_ENGINE_LOGGER_SUPPRESS_INFO
//#define ARCH_ENGINE_LOGGER_SUPPRESS_WARNING
//#define ARCH_ENGINE_LOGGER_SUPPRESS_ERROR
//#define ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG

// Library macros
#define SDL_MAIN_HANDLED


#endif	// CONFIG_ENGINE_MACROS_HPP