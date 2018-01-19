#include "LuaInterface.h"
#include <iostream>

// Allocating and initializing CLuaInterface's static data member.
// The pointer is allocated but not the object's constructor
CLuaInterface *CLuaInterface::s_instance = 0;

CLuaInterface::CLuaInterface()
	: theLuaState(NULL)
{
}

CLuaInterface::~CLuaInterface()
{
}

// Initialisation of the Lua Interface Class
bool CLuaInterface::Init()
{
	bool result = false;

	// 1. Create lua state
	theLuaState = lua_open();

	if (theLuaState)
	{
		// 2. Load lua auxiliary libraries
		luaL_openlibs(theLuaState);

		// 3. Load lua script
		luaL_dofile(theLuaState, "Lua//DataVariables.lua");

		result = true;
	}

	return result;
}

// Run the Lua Interface Class
void CLuaInterface::Run()
{
	if (theLuaState == NULL)
		return;

	// 3. Read the variables
	// lua_getglobal(lua_State*, const char*)
	lua_getglobal(theLuaState, "title");
	// extract value, index -1 as variable is already retrieved using lua_getglobal
	const char *title = lua_tostring(theLuaState, -1);

	lua_getglobal(theLuaState, "width");
	int width = lua_tointeger(theLuaState, -1);

	lua_getglobal(theLuaState, "height");
	int height = lua_tointeger(theLuaState, -1);

	// Display on screen
	std::cout << title << std::endl;
	std::cout << "------------------------------------------" << std::endl;
	std::cout << "Width of screen : " << width << std::endl;
	std::cout << "Height of screen : " << height << std::endl;
}

// Drop the Lua Interface Class
void CLuaInterface::Drop()
{
	if (theLuaState)
	{
		// Close lua state
		lua_close(theLuaState);
	}
}

// Get interger variable through the Lua Interface Class
int CLuaInterface::getIntValue(const char *dataType)
{
	lua_getglobal(theLuaState, dataType);
	return lua_tointeger(theLuaState, -1);
}

// Get float variable through the Lua Interface Class
float CLuaInterface::getFloatValue(const char *dataType)
{
	lua_getglobal(theLuaState, dataType);
	return (float)lua_tonumber(theLuaState, -1);
}

// Save interger variable through the Lua Interface Class
void CLuaInterface::saveIntValue(const char *dataType, const int value, const bool overWrite)
{
	lua_getglobal(theLuaState, "SaveToLuaFile");
	char outputString[80];
	sprintf(outputString, "%s = %d\n", dataType, value);
	lua_pushstring(theLuaState, outputString);
	lua_pushinteger(theLuaState, overWrite);
	lua_call(theLuaState, 2, 0);
}

// Save float variable through the Lua Interface Class
void CLuaInterface::saveFloatValue(const char *dataType, const float value, const bool overWrite)
{
	lua_getglobal(theLuaState, "SaveToLuaFile");
	char outputString[80];
	sprintf(outputString, "%s = %6.4f\n", dataType, value);
	lua_pushstring(theLuaState, outputString);
	lua_pushinteger(theLuaState, overWrite);
	lua_call(theLuaState, 2, 0);
}