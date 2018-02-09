#pragma once

// Includes the lua header
#include "lua.hpp"
#include "Vector3.h"

class CLuaInterface
{
protected:
	static CLuaInterface *s_instance;
	CLuaInterface();
	char *file_name;

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

	// Functions from Lua
	// Get distance square value through the Lua Interface Class
	float getDistanceSquareValue(const char *dataType, Vector3 from, Vector3 to);
	// Get variable number of values through the Lua Interface Class
	int getVariableValues(const char *dataType, int &a, int &b, int &c, int &d);

	// Get interger variable through the Lua Interface Class
	int getIntValue(const char *dataType);
	// Get float variable through the Lua Interface Class
	float getFloatValue(const char *dataType);
	// Get char value through the Lua Interface Class
	char getCharValue(const char *dataType);
	// Get Vector3 values through the Lua Interface Class
	Vector3 getVector3Values(const char *dataType);

	// Save interger variable through the Lua Interface Class
	void saveIntValue(const char *dataType, const int value, const bool overWrite = false);
	// Save float variable through the Lua Interface Class
	void saveFloatValue(const char *dataType, const float value, const bool overWrite = false);
	// Save char variable through the Lua Interface Class
	void saveCharValue(const char *dataType, const char value, const bool overWrite = false);
	// Save Vector3 variable through the Lua Interface Class
	void saveVector3Value(const char *dataType, const Vector3 value, const bool overWrite = false);
	// Save Vector3 variable through the Lua Interface Class
	void saveVector3Value(const char *function_name, const char *dataType, const Vector3 value, const bool overWrite = false);
	// Extract a field from a table
	float GetField(const char *key);

	// Set and Get file_name
	void SetFilename(char *file_name);
	char *GetFilename(void);

	// Pointer to the Lua State
	lua_State *theLuaState;
};