/*===========================================================================*
 * Arch Engine - "Script/luaBinder.hpp                                       *
 *                                                                           *
 * Class responsible for communication between the engine and Lua Scrips     *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 15/04/2018                                                       *
 * Last Modified: 15/04/2018                                                 *
 *===========================================================================*/


#ifndef SCRIPT_LUA_BINDER_HPP
#define SCRIPT_LUA_BINDER_HPP


#include "../Core/engineMacros.hpp"
#include "../Utils/serviceLocator.hpp"

extern "C" {
#if defined(__unix__)
#elif defined(_MSC_VER)
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#endif
}


#endif	// SCRIPT_LUA_BINDER_HPP
