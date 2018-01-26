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

// Get distance square value through the Lua Interface Class
float CLuaInterface::getDistanceSquareValue(const char *dataType, Vector3 from, Vector3 to)
{
	lua_getglobal(theLuaState, dataType);
	lua_pushnumber(theLuaState, from.x);
	lua_pushnumber(theLuaState, from.y);
	lua_pushnumber(theLuaState, from.z);
	lua_pushnumber(theLuaState, to.x);
	lua_pushnumber(theLuaState, to.y);
	lua_pushnumber(theLuaState, to.z);
	lua_call(theLuaState, 6, 1);
	float distanceSquare = (float)lua_tonumber(theLuaState, -1);
	lua_pop(theLuaState, 1);
	return distanceSquare;
}

// Get variable number of values through the Lua Interface Class
int CLuaInterface::getVariableValues(const char *dataType, int &a, int &b, int &c, int &d)
{
	lua_getglobal(theLuaState, dataType);
	lua_pushnumber(theLuaState, a);
	lua_pushnumber(theLuaState, b);
	lua_pushnumber(theLuaState, c);
	lua_pushnumber(theLuaState, d);
	lua_call(theLuaState, 4, 4);
	a = lua_tonumber(theLuaState, -1);
	lua_pop(theLuaState, 1);
	b = lua_tonumber(theLuaState, -1);
	lua_pop(theLuaState, 1);
	c = lua_tonumber(theLuaState, -1);
	lua_pop(theLuaState, 1);
	d = lua_tonumber(theLuaState, -1);
	lua_pop(theLuaState, 1);

	return true;
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

// Get char value through the Lua Interface Class
char CLuaInterface::getCharValue(const char *dataType)
{
	lua_getglobal(theLuaState, dataType);

	size_t len;
	const char *cstr = lua_tolstring(theLuaState, -1, &len);
	// if the string is not empty, then return the first char
	if (len > 0)
		return cstr[0];
	// else return a default value of <SPACE>
	else return ' ';
}

// Get Vector3 values through the Lua Interface Class
Vector3 CLuaInterface::getVector3Values(const char *dataType)
{
	lua_getglobal(theLuaState, dataType);
	lua_rawgeti(theLuaState, -1, 1);
	int x = lua_tonumber(theLuaState, -1);
	lua_pop(theLuaState, 1);
	lua_rawgeti(theLuaState, -1, 2);
	int y = lua_tonumber(theLuaState, -1);
	lua_pop(theLuaState, 1);
	lua_rawgeti(theLuaState, -1, 3);
	int z = lua_tonumber(theLuaState, -1);
	lua_pop(theLuaState, 1);

	return Vector3(x, y, z);
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

// Save char variable through the Lua Interface Class
void CLuaInterface::saveCharValue(const char *dataType, const char value, const bool overWrite)
{
	lua_getglobal(theLuaState, "SaveToLuaFile");
	char outputString[80];
	sprintf(outputString, "%s = %c\n", dataType, value);
	lua_pushstring(theLuaState, outputString);
	lua_pushinteger(theLuaState, overWrite);
	lua_call(theLuaState, 2, 0);
}

// Save Vector3 variable through the Lua Interface Class
void CLuaInterface::saveVector3Value(const char *dataType, const Vector3 value, const bool overWrite)
{
	lua_getglobal(theLuaState, "SaveToLuaFile");
	char outputString[80];
	sprintf(outputString, "%s = {%d,%d,%d}\n", dataType, value.x, value.y, value.z);
	lua_pushstring(theLuaState, outputString);
	lua_pushinteger(theLuaState, overWrite);
	lua_call(theLuaState, 2, 0);
}