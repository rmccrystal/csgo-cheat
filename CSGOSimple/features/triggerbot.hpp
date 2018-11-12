#pragma once

#include "includes.hpp"

namespace Triggerbot {
	void CreateMove(CUserCmd *cmd);
	C_BasePlayer* GetCrosshairPlayer(CUserCmd *cmd, int *hitBox);
}