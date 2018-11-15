#pragma once

#include <string>
#include "valve_sdk/Misc/Color.hpp"
#include <functional>
#include <Windows.h>	// for VK keys

#define OPTION(type, var, val) type var = val

enum TriggerbotStatus {
	Disabled,
	OnKey,
	Always,
	End		// Ignore
};

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

	//
	// TRIGGERBOT
	//
	OPTION(TriggerbotStatus, triggerbot_status, TriggerbotStatus::OnKey);
	OPTION(int, triggerbot_key, VK_MENU);
	OPTION(int, triggerbot_min_delay, 5);
	OPTION(int, triggerbot_max_delay, 20);

	//
	// CHAMS
	//
	OPTION(bool, chams_enabled, true);

	//
	// ESP
	//
	OPTION(bool, esp_enabled, true);
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
