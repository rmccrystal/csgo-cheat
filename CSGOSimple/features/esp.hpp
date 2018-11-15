#pragma once

#include "includes.hpp"

namespace Esp {
	class Player {
	public:
		C_BasePlayer *player;
		bool isEnemy;
		bool isVisible;
		Vector headPos;
		Vector feetPos;
		RECT bbox;
		Vector2D infoPosition;
		Vector2D infoPositionOffset;		// Offset when drawing multiple infos

		Player(C_BasePlayer *player);
		void UpdateOffset();
		void InitBBox();
		Color getColor();
		void RenderInfo(char *text);
		void RenderBox();
		void RenderName();
		void RenderHealth();
		void RenderWeaponName();
		void RenderHeaddot();
	};
	void Paint();
	RECT GetBBox(C_BaseEntity *ent);
}