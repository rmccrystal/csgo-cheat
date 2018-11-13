#pragma once

#include "includes.hpp"

namespace Triggerbot {
	void CreateMove(CUserCmd *cmd);
	std::vector<Hitboxes> GetHitboxes(C_BaseCombatWeapon* weapon);
	C_BasePlayer* GetCrosshairPlayer(CUserCmd *cmd, int *hitBox);
}