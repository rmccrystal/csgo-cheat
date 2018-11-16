#include "crosshair.hpp"

static int crosshairSize = 6;

void Crosshair::Paint()
{
	if (!g_Options.crosshair_enabled)
		return;

	if (!g_LocalPlayer->IsAlive())
		return;

	Vector crosshairPos = GetCrosshairPos();
	
	static int height, width;
	g_EngineClient->GetScreenSize(width, height);

	Vector screenMiddle(width/2, height/2, 0);
	if (screenMiddle.DistTo(crosshairPos) <= 5)		// If the crosshair is right on the real crosshair there is no need to show it
		return;

	static float fadeDist = 50;

	DrawCrosshair(GetCrosshairPos(), min(255*(screenMiddle.DistTo(crosshairPos)-5.0)/fadeDist, 255));
}

Vector Crosshair::GetCrosshairPos()
{
	return GetCrosshairPos2();
}

Vector Crosshair::GetCrosshairPos1()
{
	QAngle angles = *g_LocalPlayer->GetVAngles() + (g_LocalPlayer->m_aimPunchAngle()* 2.f);
	Vector forward;

	Math::AngleVectors(angles, forward);

	forward *= 8192;

	Vector startPos = g_LocalPlayer->GetEyePos();
	Vector endPos = startPos + forward;
	Vector screenPos;

	Math::WorldToScreen(endPos, screenPos);

	return screenPos;

}

Vector Crosshair::GetCrosshairPos2()
{
	static int height, width;
	g_EngineClient->GetScreenSize(width, height);

	QAngle punchAngles = g_LocalPlayer->m_aimPunchAngle();

	int x = width / 2;
	int y = height / 2;
	int dy = height / 90;
	int dx = width / 90;
	x -= (dx*(punchAngles.yaw));
	y += (dy*(punchAngles.pitch));
	
	return Vector(x, y, 0);
}

void Crosshair::DrawCrosshair(Vector location, int transparency)
{
	DrawCrosshair(location.x, location.y, transparency);
}

void Crosshair::DrawCrosshair(int x, int y, int transparency)
{
	Draw::Rectangle(x - 1, y - crosshairSize - 1,
		x + 2, y + crosshairSize + 2,
		Color(0, 0, 0, transparency));
	Draw::Rectangle(x - crosshairSize - 1, y - 1,
		x + crosshairSize + 2, y + 2,
		Color(0, 0, 0, transparency));
	Draw::Line(x - crosshairSize, y, x + crosshairSize + 1, y, Color(g_Options.crosshair_color.r(), g_Options.crosshair_color.g(), g_Options.crosshair_color.b(), transparency));		// --
	Draw::Line(x, y - crosshairSize, x, y + crosshairSize + 1, Color(g_Options.crosshair_color.r(), g_Options.crosshair_color.g(), g_Options.crosshair_color.b(), transparency));		// |
}
