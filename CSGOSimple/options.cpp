#include "options.hpp"

#include <vector>
#include <Windows.h>
#include <algorithm>
#include "helpers/utils.hpp"

Config g_Options;
bool   g_Unload = false;

struct keyBind {
	int keyCode;
	bool ready;			// Ready for the next keypress
	std::function<void(void)> function;
};

std::vector<keyBind> keyBinds;

void Keybinds::Paint()
{
	for (unsigned int i = 0; i < keyBinds.size(); i++) {
		keyBind *bind = &keyBinds[i];
		if (GetKeyState(bind->keyCode) & 0x8000) {	// If the key is pressed
			if (bind->ready) {
				bind->function();
			}
			bind->ready = false;
		}
		else {
			bind->ready = true;
		}
	}
}

void Keybinds::InitHotkeys()
{
	RegisterBoolHotkey(VK_F6, &g_Options.bhop_enabled);
}

void Keybinds::RegisterHotkey(int keyCode, std::function<void(void)> func)
{
	keyBind bind;
	bind.keyCode = keyCode;
	bind.function = func;
	bind.ready = false;
	keyBinds.push_back(bind);
}

// Doesn't work
void Keybinds::RemoveHotkey(int keyCode)	// TODO: FIX this function it doesnt work
{
	for (keyBind bind : keyBinds) {
		if (bind.keyCode == keyCode) {
			//keyBinds.erase(std::remove(keyBinds.begin(), keyBinds.end(), bind), keyBinds.end());	// Remove keybind
		}
	}
}

void Keybinds::RegisterBoolHotkey(int keyCode, bool * boolPtr)
{
	RegisterHotkey(keyCode, [=]() {
		*boolPtr = !*boolPtr;
	});
}
