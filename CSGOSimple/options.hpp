#pragma once

#include <string>
#include "valve_sdk/Misc/Color.hpp"

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
