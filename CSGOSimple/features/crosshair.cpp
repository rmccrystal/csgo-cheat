#include "crosshair.hpp"

static int crosshairSize = 10;

void Crosshair::Paint()
{
	if (!g_Options.crosshair_enabled)
		return;

	//Draw::Rectangle(50, 50, 100, 150, Color(255, 255, 255));
	DrawCrosshair(500, 500);
}

void Crosshair::DrawCrosshair(int x, int y)
{
	Draw::Line(x - crosshairSize, y, x + crosshairSize, y, g_Options.crosshair_color);		// --
	Draw::Line(x, y - crosshairSize, x, y + crosshairSize, g_Options.crosshair_color);		// |
}
