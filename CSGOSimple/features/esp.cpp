#include "esp.hpp"

void Esp::Paint()
{
	if (!g_Options.esp_enabled)
		return;

	for (int i = 1; i <= g_EntityList->GetMaxEntities(); i++) {
		C_BaseEntity *ent = C_BaseEntity::GetEntityByIndex(i);

		if (!ent)
			continue;

		if (ent->IsPlayer()) {
			C_BasePlayer *entPlayer = static_cast<C_BasePlayer*>(ent);

			if (!entPlayer->IsAlive() || entPlayer->EntIndex() == g_LocalPlayer->EntIndex())	// If player is dead or self
				continue;

			Player player(entPlayer);

			player.RenderBox();
			player.RenderHeaddot();
		}
	}
}

RECT Esp::GetBBox(C_BaseEntity* ent)
{
	RECT rect{};
	auto collideable = ent->GetCollideable();

	if (!collideable)
		return rect;

	auto min = collideable->OBBMins();
	auto max = collideable->OBBMaxs();

	const matrix3x4_t& trans = ent->m_rgflCoordinateFrame();

	Vector points[] = {
		Vector(min.x, min.y, min.z),
		Vector(min.x, max.y, min.z),
		Vector(max.x, max.y, min.z),
		Vector(max.x, min.y, min.z),
		Vector(max.x, max.y, max.z),
		Vector(min.x, max.y, max.z),
		Vector(min.x, min.y, max.z),
		Vector(max.x, min.y, max.z)
	};

	Vector pointsTransformed[8];
	for (int i = 0; i < 8; i++) {
		Math::VectorTransform(points[i], trans, pointsTransformed[i]);
	}

	Vector screen_points[8] = {};

	for (int i = 0; i < 8; i++) {
		if (!Math::WorldToScreen(pointsTransformed[i], screen_points[i]))
			return rect;
	}

	auto left = screen_points[0].x;
	auto top = screen_points[0].y;
	auto right = screen_points[0].x;
	auto bottom = screen_points[0].y;

	for (int i = 1; i < 8; i++) {
		if (left > screen_points[i].x)
			left = screen_points[i].x;
		if (top < screen_points[i].y)
			top = screen_points[i].y;
		if (right < screen_points[i].x)
			right = screen_points[i].x;
		if (bottom > screen_points[i].y)
			bottom = screen_points[i].y;
	}
	return RECT{ (long)left, (long)top, (long)right, (long)bottom };
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

	/*
	this->bbox.top = player->GetHitboxPos(HITBOX_HEAD).y;
	this->bbox.bottom = min(player->GetHitboxPos(HITBOX_LEFT_FOOT).y, player->GetHitboxPos(HITBOX_RIGHT_FOOT).y);
	this->bbox.left = min(player->GetHitboxPos(HITBOX_LEFT_HAND).x, player->GetHitboxPos(HITBOX_RIGHT_HAND).x);
	this->bbox.right = max(player->GetHitboxPos(HITBOX_LEFT_HAND).x, player->GetHitboxPos(HITBOX_RIGHT_HAND).x);
	*/
	this->bbox = GetBBox(player);
}

void Esp::Player::RenderBox()
{
	Draw::Rectangle(bbox.left, bbox.top, bbox.right, bbox.bottom, Color(255, 255, 255));
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
	Vector screenLocation;
	Math::WorldToScreen(this->headPos, screenLocation);
	Draw::FilledCircle(Vector2D(screenLocation.x, screenLocation.y), 30, 1, Color(255, 255, 255));
}
