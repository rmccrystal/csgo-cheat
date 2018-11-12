#include "menu.hpp"

HFont Menu::Font;

Vector2D menuPosition(200, 25);
int nextItemOffset = 0;		// Ofset for drawing the next menu item
int itemDistance = 20;		// Distance between each item
int statusDistance = 100;	// Distance between the item and the status

void Menu::Paint()
{
	Draw::Text(5, 5, "balterhook", Font, Color(255, 255, 255));

	DrawItems();
}

void Menu::DrawItems()
{
	nextItemOffset = 0;

	DrawBoolItem("Bhop:", g_Options.bhop_enabled);
}

void Menu::DrawItem(const char * text, const char * status, Color statusColor)
{
	Color color = Color(255, 255, 255);
	Draw::Text(menuPosition.x, menuPosition.y + nextItemOffset, text, Font, color);
	Draw::Text(menuPosition.x + statusDistance, menuPosition.y + nextItemOffset, status, Font, statusColor);

	nextItemOffset += itemDistance;
}

void Menu::DrawBoolItem(const char *text, bool enabled)
{
	if (enabled) {
		DrawItem(text, "ON", Color(0, 255, 255));
	}
	else {
		DrawItem(text, "OFF", Color(255, 255, 255));
	}
}