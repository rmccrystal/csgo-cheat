#include "triggerbot.hpp"

void Triggerbot::CreateMove(CUserCmd * cmd)
{
	if (!g_Options.triggerbot_enabled)
		return;

	if (!g_LocalPlayer || !g_LocalPlayer->IsAlive())
		return;

	if (g_LocalPlayer->GetFlashBangTime() - g_GlobalVars->curtime > 2.0f)	// If player is flashbanged
		return;

	int hitBox;
	C_BasePlayer *player = GetCrosshairPlayer(cmd, &hitBox);

	if (!player)
		return;
	// TODO: Smoke check
	if (hitBox == Hitboxes::HITBOX_HEAD || hitBox == Hitboxes::HITBOX_CHEST || hitBox == Hitboxes::HITBOX_LOWER_CHEST ||
		hitBox == Hitboxes::HITBOX_UPPER_CHEST || hitBox == Hitboxes::HITBOX_NECK || hitBox == Hitboxes::HITBOX_PELVIS ||
		hitBox == Hitboxes::HITBOX_STOMACH) {
		cmd->buttons |= IN_ATTACK;
	}
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
