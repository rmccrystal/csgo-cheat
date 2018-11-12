#pragma once

#include "includes.hpp"

namespace Menu {
	extern HFont Font;
	void Paint();
	void DrawItems();
	void DrawItem(const char *text, const char *status, Color statusColor);
	void DrawBoolItem(const char *text, bool enabled);
}