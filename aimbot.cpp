#include "aimbot.h"
#include "lolhack.h"
#include "utils.h"

//SH_DECL_MANUALHOOK0(MHook_Kuk, player, offset, 0, int);
//SH_ADD_MANUALHOOK(MHook_Function2, iface, SH_STATIC(Hook_Function2), false);
int Aimbot::GetPlayerTeam(IClientEntity *player)
{
	if (!player)
		return 0;

	RecvProp *prop = GetRecvProp("m_iTeamNum", player->GetClientClass()->m_pRecvTable);
	//if (!prop)
	//	return 0;

	int offset = GetEntry("DT_BaseEntity", "m_iTeamNum", player->GetClientClass()->m_pRecvTable);

 	return offset;
}

void Aimbot::Run()
{
	if (!g_AimbotEnabled)
		return;

	IClientEntity *localplayer = GetLocalPlayer();
	if (!localplayer)
		return;

	Vector localOrigin = localplayer->GetAbsOrigin();
	QAngle viewAngles;
	g_engineClient->GetViewAngles(viewAngles);

	int shortestDelta = 999999;
	static IClientEntity *currentTarget;
	for (int i = 1; i < g_engineClient->GetMaxClients(); ++i)
	{
		IClientEntity* player = g_entityList->GetClientEntity(i);

		if (!player
			|| player == localplayer
			|| player->IsDormant()
			|| !player->ShouldDraw()
			|| !CanSeePlayer(player))
		{
			if (currentTarget == player)
			{
				currentTarget = nullptr;
				shortestDelta = 999999;
			}
			continue;
		}

		char buf[64];
		int team = GetPlayerTeam(player);
		sprintf(buf, "echo player %i team %i\n", i, team);
		//engineClient->ClientCmd_Unrestricted(buf);

		localOrigin = localplayer->GetAbsOrigin();
		Vector playerOrigin = player->GetAbsOrigin();

		int delta = localOrigin.DistTo(playerOrigin);
		QAngle finalAngle = CalcAngle(localOrigin, playerOrigin - Vector(0, 0, 15));
		if (!currentTarget || delta <= shortestDelta && GetFov(viewAngles, finalAngle) < 50)
		{
			currentTarget = player;
			shortestDelta = delta;
			//break;
		}
	}

	if (currentTarget)
	{
		Vector targetVec = currentTarget->GetAbsOrigin() - Vector(0, 0, 15);
		QAngle finalAngle = CalcAngle(localOrigin, targetVec);
		
		int delta = localOrigin.DistTo(targetVec);

		if (delta < 1024 && GetFov(viewAngles, finalAngle) < 50)
			g_engineClient->SetViewAngles(finalAngle);

	}
}