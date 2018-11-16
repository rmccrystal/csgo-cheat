#pragma once

#include "includes.hpp"

namespace Crosshair {
	void Paint();
	Vector GetCrosshairPos();
	Vector GetCrosshairPos1();
	Vector GetCrosshairPos2();
	void DrawCrosshair(Vector location, int transparency);
	void DrawCrosshair(int x, int y, int transparency);
}