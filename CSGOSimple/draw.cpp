#include "draw.hpp"
#include "valve_sdk/sdk.hpp"
#include "helpers/utils.hpp"

#define M_PI 3.1415926

void Draw::Circle(Vector2D position, float points, float radius, Color color)
{
	float step = (float)M_PI * 2.0f / points;

	for (float a = 0; a < (M_PI * 2.0f); a += step)
	{
		Vector2D start(radius * cosf(a) + position.x, radius * sinf(a) + position.y);
		Vector2D end(radius * cosf(a + step) + position.x, radius * sinf(a + step) + position.y);
		Line(start, end, color);
	}
}

void Draw::FilledCircle(Vector2D position, float points, float radius, Color color)
{
	std::vector<Vertex_t> vertices;
	float step = (float)M_PI * 2.0f / points;

	for (float a = 0; a < (M_PI * 2.0f); a += step)
		vertices.push_back(Vertex_t(Vector2D(radius * cosf(a) + position.x, radius * sinf(a) + position.y)));

	Draw::TexturedPolygon((int)points, vertices.data(), color);
}

void Draw::Circle3D(Vector position, float points, float radius, Color color)
{
	float step = (float)M_PI * 2.0f / points;

	std::vector<Vector> points3d;
	for (float a = 0; a < (M_PI * 2.0f); a += step)
	{
		Vector start(radius * cosf(a) + position.x, radius * sinf(a) + position.y, position.z);
		Vector end(radius * cosf(a + step) + position.x, radius * sinf(a + step) + position.y, position.z);

		Vector start2d, end2d;
		if (g_DebugOverlay->ScreenPosition(start, start2d) || g_DebugOverlay->ScreenPosition(end, end2d))
			return;

		Draw::Line(Vector2D(start2d.x, start2d.y), Vector2D(end2d.x, end2d.y), color);
	}
}

void Draw::FilledRectangle(int x0, int y0, int x1, int y1, Color col)
{
	g_VGuiSurface->DrawSetColor(col);
	g_VGuiSurface->DrawFilledRect(x0, y0, x1, y1);
}

void Draw::FilledRectangle(Vector2D start_pos, Vector2D end_pos, Color col)
{
	FilledRectangle(start_pos.x, start_pos.y, end_pos.x, end_pos.y, col);
}

void Draw::Rectangle(int x0, int y0, int x1, int y1, Color col)
{
	g_VGuiSurface->DrawSetColor(col);
	g_VGuiSurface->DrawOutlinedRect(x0, y0, x1, y1);
}

void Draw::Rectangle(Vector2D start_pos, Vector2D end_pos, Color col)
{
	Rectangle(start_pos.x, start_pos.y, end_pos.x, end_pos.y, col);
}

void Draw::Line(int x0, int y0, int x1, int y1, Color col)
{
	g_VGuiSurface->DrawSetColor(col);
	g_VGuiSurface->DrawLine(x0, y0, x1, y1);
}
void Draw::Line(Vector2D start_pos, Vector2D end_pos, Color col)
{
	Line(start_pos.x, start_pos.y, end_pos.x, end_pos.y, col);
}

void Draw::PolyLine(int* px, int* py, int num_points, Color col)
{
	g_VGuiSurface->DrawSetColor(col);
	g_VGuiSurface->DrawPolyLine(px, py, num_points);
}

void Draw::PolyLine(Vertex_t* vertice, int num_points, Color col)
{
	static int* points_x = new int[num_points];
	static int* points_y = new int[num_points];
	for (int i = 0; i < num_points; i++)
	{
		points_x[i] = static_cast<int>(vertice[i].m_Position.x);
		points_y[i] = static_cast<int>(vertice[i].m_Position.y);
	}
	PolyLine(points_x, points_y, num_points, col);
}

void Draw::TexturedPolygon(int n, Vertex_t* vertice, Color col)
{
	static int texture_id = g_VGuiSurface->CreateNewTextureID(true);
	static unsigned char buf[4] = { 255, 255, 255, 255 };
	g_VGuiSurface->DrawSetTextureRGBA(texture_id, buf, 1, 1);
	g_VGuiSurface->DrawSetColor(col);
	g_VGuiSurface->DrawSetTexture(texture_id);
	g_VGuiSurface->DrawTexturedPolygon(n, vertice);
}

void Draw::TextW(int x, int y, const wchar_t* text, HFont font, Color col)
{
	g_VGuiSurface->DrawSetTextPos(x, y);
	g_VGuiSurface->DrawSetTextFont(font);
	g_VGuiSurface->DrawSetTextColor(col);
	g_VGuiSurface->DrawPrintText(text, wcslen(text));
}

void Draw::TextW(Vector2D pos, const wchar_t* text, HFont font, Color col)
{
	TextW(pos.x, pos.y, text, font, col);
}

void Draw::Text(int x, int y, const char* text, HFont font, Color col)
{
	std::wstring wc = Utils::StringToWstring(text);

	g_VGuiSurface->DrawSetTextPos(x, y);
	g_VGuiSurface->DrawSetTextFont(font);
	g_VGuiSurface->DrawSetTextColor(col);
	g_VGuiSurface->DrawPrintText(wc.c_str(), wcslen(wc.c_str()));
}

void Draw::Text(Vector2D pos, const char* text, HFont font, Color col)
{
	Text(pos.x, pos.y, text, font, col);
}

void Draw::GetTextWSize(const wchar_t* text, HFont font, int& wide, int& tall)
{
	g_VGuiSurface->GetTextSize(font, text, wide, tall);
}

void Draw::GetTextSize(const char* text, HFont font, int& wide, int& tall)
{
	std::wstring wc = Utils::StringToWstring(text);

	g_VGuiSurface->GetTextSize(font, wc.c_str(), wide, tall);
}

Vector2D Draw::GetTextWSize(const wchar_t* text, HFont font)
{
	int x_res, y_res;
	g_VGuiSurface->GetTextSize(font, text, x_res, y_res);
	return Vector2D(x_res, y_res);
}

Vector2D Draw::GetTextSize(const char* text, HFont font)
{
	std::wstring wc = Utils::StringToWstring(text);

	int x_res, y_res;
	g_VGuiSurface->GetTextSize(font, wc.c_str(), x_res, y_res);
	return Vector2D(x_res, y_res);
}

HFont Draw::CreateDxFont(const char* fontName, int size, int flag)
{
	HFont newFont = g_VGuiSurface->CreateFont_();
	g_VGuiSurface->SetFontGlyphSet(newFont, fontName, size, 0, 0, 0, flag);
	return newFont;
}

namespace Fonts {
	HFont MenuFont;
	HFont ESPFont;

	void Init()
	{
		MenuFont = Draw::CreateDxFont("Calibri Bold", 16, FontFlags::FONTFLAG_OUTLINE);
		ESPFont = Draw::CreateDxFont("Calibri Bold", 16, FontFlags::FONTFLAG_OUTLINE);
	}
}
