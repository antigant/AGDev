#pragma once

// Includes the lua header
#include "lua.hpp"

class CLuaInterface
{
protected:
	static CLuaInterface *s_instance;
	CLuaInterface();

public:
	static CLuaInterface *GetInstance()
	{
		if (!s_instance)
			s_instance = new CLuaInterface;
		return s_instance;
	}
	static bool DropInstance()
	{
		if (s_instance)
		{
			// Drop the Lua Interface class
			s_instance->Drop();

			delete s_instance;
			s_instance = NULL;
			return true;
		}
		return false;
	}
	virtual ~CLuaInterface();

	// Initialisation of the Lua Interface Class
	bool Init();

	// Run the Lua Interface Class
	void Run();

	// Drop the Lua Interface Class
	void Drop();

	// Get interger variable through the Lua Interface Class
	int getIntValue(const char *dataType);
	// Get float variable through the Lua Interface Class
	float getFloatValue(const char *dataType);

	// Save interger variable through the Lua Interface Class
	void saveIntValue(const char *dataType, const int value, const bool overWrite = false);
	// Save float variable through the Lua Interface Class
	void saveFloatValue(const char *dataType, const float value, const bool overWrite = false);

	// Pointer to the Lua State
	lua_State *theLuaState;
};