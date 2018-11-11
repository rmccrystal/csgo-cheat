#include "menu.hpp"

HFont Menu::Font;

void Menu::Paint()
{
	Draw::Text(10, 10, "balterhook", Font, Color(255, 255, 255));
}
