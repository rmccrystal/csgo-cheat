#include "esp.hpp"

void Esp::Paint()
{
	if (!g_Options.esp_enabled)
		return;


}

Esp::Player::Player(C_BasePlayer * player)
{
	this->player = player;
	this->isEnemy = g_LocalPlayer->m_iTeamNum() != player->m_iTeamNum();
	this->isVisible = g_LocalPlayer->CanSeePlayer(player, HITBOX_HEAD) || g_LocalPlayer->CanSeePlayer(player, HITBOX_CHEST);

	this->headPos = player->GetHitboxPos(HITBOX_HEAD);
	this->feetPos = Vector(((player->GetHitboxPos(HITBOX_LEFT_FOOT).x + player->GetHitboxPos(HITBOX_RIGHT_FOOT).x) / 2),
		((player->GetHitboxPos(HITBOX_LEFT_FOOT).y + player->GetHitboxPos(HITBOX_RIGHT_FOOT).y) / 2),
		((player->GetHitboxPos(HITBOX_LEFT_FOOT).z + player->GetHitboxPos(HITBOX_RIGHT_FOOT).z) / 2)); // Average each foot pos

	// set bbox
}

void Esp::Player::RenderBox()
{
}

void Esp::Player::RenderName()
{
}

void Esp::Player::RenderHealth()
{
}

void Esp::Player::RenderWeaponName()
{
}

void Esp::Player::RenderHeaddot()
{
}
