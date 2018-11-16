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

			std::unique_ptr<Player> player = std::make_unique<Player>(entPlayer);

			player->RenderBox();
			//player->RenderHeaddot();		// Currently doesn't work //TODO: FIX
			
			player->RenderHealth();
			player->RenderWeaponName();
			//player->RenderName();
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
	this->isVisible = g_LocalPlayer->CanSeePlayer(player, HITBOX_HEAD) || g_LocalPlayer->CanSeePlayer(player, HITBOX_CHEST) || g_LocalPlayer->CanSeePlayer(player, HITBOX_RIGHT_FOOT) || g_LocalPlayer->CanSeePlayer(player, HITBOX_LEFT_FOOT);
	this->headPos = player->GetHitboxPos(HITBOX_HEAD);
	this->feetPos = Vector(((player->GetHitboxPos(HITBOX_LEFT_FOOT).x + player->GetHitboxPos(HITBOX_RIGHT_FOOT).x) / 2),
		((player->GetHitboxPos(HITBOX_LEFT_FOOT).y + player->GetHitboxPos(HITBOX_RIGHT_FOOT).y) / 2),
		((player->GetHitboxPos(HITBOX_LEFT_FOOT).z + player->GetHitboxPos(HITBOX_RIGHT_FOOT).z) / 2)); // Average each foot pos
	
	this->infoPositionOffset = Vector2D(0, 0);

	InitBBox();

	this->infoPosition = Vector2D(bbox.right, bbox.bottom);

	/*
	this->bbox.top = player->GetHitboxPos(HITBOX_HEAD).y;
	this->bbox.bottom = min(player->GetHitboxPos(HITBOX_LEFT_FOOT).y, player->GetHitboxPos(HITBOX_RIGHT_FOOT).y);
	this->bbox.left = min(player->GetHitboxPos(HITBOX_LEFT_HAND).x, player->GetHitboxPos(HITBOX_RIGHT_HAND).x);
	this->bbox.right = max(player->GetHitboxPos(HITBOX_LEFT_HAND).x, player->GetHitboxPos(HITBOX_RIGHT_HAND).x);
	*/
}

void Esp::Player::UpdateOffset()
{
	this->infoPositionOffset.y += 12;
}

void Esp::Player::InitBBox()
{
	auto head = player->GetHitboxPos(HITBOX_HEAD);
	auto origin = player->m_vecOrigin();

	head.z += 8;	// Fix for weird hitboxes

	if (!Math::WorldToScreen(head, headPos) ||
		!Math::WorldToScreen(origin, feetPos))
		return;

	auto h = fabs(headPos.y - feetPos.y);
	auto w = h / 1.65f;

	bbox.left = static_cast<long>(feetPos.x - w * 0.5f);
	bbox.right = static_cast<long>(bbox.left + w);
	bbox.bottom = static_cast<long>(feetPos.y);
	bbox.top = static_cast<long>(headPos.y);
}

Color Esp::Player::getColor()
{
	static float notVisibleSaturation = 0.45;

	float saturation;
	if (isVisible)
		saturation = 1;
	else
		saturation = notVisibleSaturation;

	Color color = color.FromHSB((player->m_iHealth() / 100.0) * 0.333, saturation, 1);
	return color;
}

void Esp::Player::RenderInfo(char * text)
{
	Draw::Text(infoPosition + infoPositionOffset, text, Fonts::ESPFont, getColor());
	UpdateOffset();
}

void Esp::Player::RenderInfo(std::string text)
{
	char buff[1024];
	strcpy(buff, text.c_str());
	RenderInfo(buff);

}

void Esp::Player::RenderBox()
{
	static int invisibleBrightness = 45;

	Color outlineColor;
	if (isVisible)
		outlineColor = Color(0, 0, 0);
	else
		outlineColor = Color(invisibleBrightness, invisibleBrightness, invisibleBrightness);

	Draw::OutlinedRectangle(bbox.left, bbox.top, bbox.right, bbox.bottom, getColor(), outlineColor);
}

void Esp::Player::RenderName()
{
	RenderInfo(player->GetPlayerInfo().szName);
}

void Esp::Player::RenderHealth()
{
	std::string health = std::string("HP: ") + std::to_string(player->m_iHealth());
	RenderInfo(health);
}

void Esp::Player::RenderWeaponName()
{
	RenderInfo(Utils::GetWeaponName(player->m_hActiveWeapon()->GetCSWeaponData()));
}

void Esp::Player::RenderHeaddot()
{
	Vector screenLocation;
	Math::WorldToScreen(this->headPos, screenLocation);
	Draw::FilledCircle(Vector2D(screenLocation.x, screenLocation.y), 10, 1, Color(255, 255, 255));
}
