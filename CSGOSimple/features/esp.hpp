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

		Player(C_BasePlayer *player);
		void RenderBox();
		void RenderName();
		void RenderHealth();
		void RenderWeaponName();
		void RenderHeaddot();
	};
	void Paint();
	RECT GetBBox(C_BaseEntity *ent);
}