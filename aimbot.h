#pragma once
#include "interfaces.h"

class Aimbot
{
public:
	static void Run();

	static int GetPlayerTeam(IClientEntity *player);
};