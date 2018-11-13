#include "triggerbot.hpp"

#include <ctime>

int nextShootTime = 0;
static float maxSpread = 0.025;

void Triggerbot::CreateMove(CUserCmd * cmd)
{

	if (!g_Options.triggerbot_enabled)
		return;

	if (!g_LocalPlayer || !g_LocalPlayer->IsAlive())
		return;

	if (nextShootTime == 0) {
		int hitBox;	// Update next shoot tick
		C_BasePlayer *player = GetCrosshairPlayer(cmd, &hitBox);

		if (!player)
			return;
		// TODO: Smoke check

		if (g_LocalPlayer->m_hActiveWeapon()->GetInaccuracy() >= maxSpread)
			return;

		std::vector<Hitboxes> hitboxes = GetHitboxes(g_LocalPlayer->m_hActiveWeapon());

		if (std::end(hitboxes) != std::find(std::begin(hitboxes), std::end(hitboxes), hitBox)) {
			int msToAdd = rand() % (g_Options.triggerbot_max_delay - g_Options.triggerbot_min_delay + 1) + g_Options.triggerbot_min_delay;
			nextShootTime = Utils::getMsTime() + msToAdd;
		}
	}
	else if (nextShootTime <= Utils::getMsTime()) {
		cmd->buttons |= IN_ATTACK;
		nextShootTime = 0;
	}
}

std::vector<Hitboxes> Triggerbot::GetHitboxes(C_BaseCombatWeapon * weapon)
{
	std::vector<Hitboxes> hitboxes;
	
	if (!weapon->IsGun())
		return hitboxes;	// Return nothing

	hitboxes.push_back(Hitboxes::HITBOX_HEAD);	// All weapons should shoot head

	switch (weapon->GetCSWeaponData()->iWeaponType) {
	case WEAPONTYPE_PISTOL:
		return hitboxes;
	case WEAPONTYPE_RIFLE:
	case WEAPONTYPE_SUBMACHINEGUN:
	case WEAPONTYPE_MACHINEGUN:
		hitboxes.push_back(Hitboxes::HITBOX_NECK);
		return hitboxes;
	case WEAPONTYPE_SHOTGUN:
		hitboxes.push_back(Hitboxes::HITBOX_UPPER_CHEST);
		hitboxes.push_back(Hitboxes::HITBOX_CHEST);
		hitboxes.push_back(Hitboxes::HITBOX_LOWER_CHEST);
		hitboxes.push_back(Hitboxes::HITBOX_NECK);
		hitboxes.push_back(Hitboxes::HITBOX_PELVIS);
		hitboxes.push_back(Hitboxes::HITBOX_STOMACH);
		return hitboxes;
	case WEAPONTYPE_SNIPER_RIFLE:
		if (weapon->GetClientClass()->m_ClassID == ClassId::ClassId_CWeaponSSG08)
			return hitboxes; // only head for scout
		if (weapon->GetClientClass()->m_ClassID == ClassId::ClassId_CWeaponAWP) {
			hitboxes.push_back(Hitboxes::HITBOX_UPPER_CHEST);
			hitboxes.push_back(Hitboxes::HITBOX_CHEST);
			hitboxes.push_back(Hitboxes::HITBOX_LOWER_CHEST);
			hitboxes.push_back(Hitboxes::HITBOX_NECK);
			hitboxes.push_back(Hitboxes::HITBOX_PELVIS);
			hitboxes.push_back(Hitboxes::HITBOX_STOMACH);
			return hitboxes;
		}
		else {
			// Autosniper
			return hitboxes;
		}
	}

	return hitboxes;
}

C_BasePlayer* Triggerbot::GetCrosshairPlayer(CUserCmd *cmd, int * hitBox = nullptr)
{
	Vector src, dst, forward;
	trace_t tr;
	Ray_t ray;
	CTraceFilter filter;

	QAngle viewangle = cmd->viewangles;
	
	viewangle += g_LocalPlayer->m_aimPunchAngle() * 2.f;

	Math::AngleVectors(viewangle, forward);
	forward *= g_LocalPlayer->m_hActiveWeapon()->GetCSWeaponData()->flRange;
	filter.pSkip = g_LocalPlayer;
	src = g_LocalPlayer->GetEyePos();
	dst = src + forward;

	ray.Init(src, dst);

	g_EngineTrace->TraceRay(ray, 0x46004003, &filter, &tr);

	if (!tr.hit_entity)
		return nullptr;

	C_BasePlayer *player = static_cast<C_BasePlayer*>(tr.hit_entity);

	if (!player)
		return nullptr;

	if (!player->IsPlayer())
		return nullptr;

	if (player == g_LocalPlayer
		|| player->IsDormant()
		|| !player->IsAlive())
		return nullptr;

	*hitBox = tr.hitbox;

	return player;
}
