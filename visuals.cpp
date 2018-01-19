#include "visuals.h"

Visuals *g_Visuals;
void Visuals::RenderCrosshair()
{
	int w, h;

	g_engineClient->GetScreenSize(w, h);

	g_vguiSurface->DrawSetColor(255, 0, 128, 255);

	int cx = w / 2;
	int cy = h / 2;

	g_vguiSurface->DrawLine(cx - 25, cy, cx + 25, cy);
	g_vguiSurface->DrawLine(cx, cy - 25, cx, cy + 25);
}

void Visuals::RenderTitle()
{
	wchar_t buf[128] = L"hi i'm hack please no report!";

	if (!font)
	{
		font = g_vguiSurface->CreateFont();
		g_vguiSurface->SetFontGlyphSet(font, "Arial", 15, 100, 0, 0, 0);
	}

	g_vguiSurface->DrawSetTextFont(font);
	g_vguiSurface->DrawSetTextColor(255, 128, 128, 255);
	g_vguiSurface->DrawSetTextPos(10, 10);
	g_vguiSurface->DrawPrintText(buf, wcslen(buf));
}

void Visuals::RenderEntity(IClientEntity *entity)
{
	auto head = entity->GetAbsOrigin() + Vector(0,0,64);
	auto origin = entity->GetAbsOrigin();
	auto w = 48;

	head.x -= w / 2;
	origin.x += w / 2;

	Vector screenHead, screenFeet;

	if (!WorldToScreen(head, screenHead) ||
		!WorldToScreen(origin, screenFeet))
		return;


	int x = screenHead.x;
	int y = screenFeet.y;

	g_vguiSurface->DrawSetColor(0,255,0,255);
	g_vguiSurface->DrawOutlinedRect(screenHead.x, screenHead.y, screenFeet.x, screenFeet.y);

	player_info_s *plr = new player_info_s();
	g_engineClient->GetPlayerInfo(entity->entindex(), plr);

	g_vguiSurface->DrawSetTextPos(screenHead.x, screenHead.y);

	wchar_t buf[128];
	mbstowcs(buf, plr->name, strlen(plr->name));
	g_vguiSurface->DrawPrintText(buf, wcslen(buf));
}

