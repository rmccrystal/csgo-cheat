#pragma once

#include <string>
#include "valve_sdk/Misc/Color.hpp"
#include <functional>

#define OPTION(type, var, val) type var = val

class Config
{
public:

	//
	// Menu
	//
	OPTION(bool, menu_enabled, true);

	//
	// BHOP
	//
	OPTION(bool, bhop_enabled, false);
};

extern Config g_Options;
extern bool   g_Unload;

namespace Keybinds {
	void Paint();
	void InitHotkeys();
	void RegisterHotkey(int keyCode, std::function<void(void)> func);
	void RemoveHotkey(int keyCode);
	void RegisterBoolHotkey(int keyCode, bool *boolPtr);
}
