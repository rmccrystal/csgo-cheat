#include "menu.hpp"

HFont Menu::Font;

Vector2D menuPosition(250, 50);
int nextItemOffset = 0;		// Ofset for drawing the next menu item
int itemDistance = 12;		// Distance between each item
int statusDistance = 100;	// Distance between the item and the status

Color onColor = Color(0, 255, 255);
Color offColor = Color(255, 255, 255);

void Menu::Paint()
{
	Draw::Text(20, 50, "balterhook", Fonts::MenuFont, Color(255, 255, 255));
	DrawItems();
}

void Menu::DrawItems()
{
	nextItemOffset = 0;

	DrawBoolItem("Bhop:", g_Options.bhop_enabled);
	switch (g_Options.triggerbot_status) {
	case TriggerbotStatus::Always:
		DrawItem("Triggerbot:", "Always", onColor);
		break;
	case TriggerbotStatus::OnKey:
		DrawItem("Triggerbot:", "On Key", onColor);
		break;
	case TriggerbotStatus::Disabled:
		DrawItem("Triggerbot:", "OFF", offColor);
		break;
	}
	DrawBoolItem("Esp:", g_Options.esp_enabled);
}

void Menu::DrawItem(const char * text, const char * status, Color statusColor)
{
	Color color = Color(255, 255, 255);
	Draw::Text(menuPosition.x, menuPosition.y + nextItemOffset, text, Fonts::MenuFont, color);
	Draw::Text(menuPosition.x + statusDistance, menuPosition.y + nextItemOffset, status, Fonts::MenuFont, statusColor);

	nextItemOffset += itemDistance;
}

void Menu::DrawBoolItem(const char *text, bool enabled)
{
	if (enabled) {
		DrawItem(text, "ON", onColor);
	}
	else {
		DrawItem(text, "OFF", offColor);
	}
}