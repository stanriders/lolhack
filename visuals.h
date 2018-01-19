#pragma once
#include "interfaces.h"
#include "utils.h"

class Visuals
{
public:
	void RenderCrosshair();
	void RenderTitle();
	void RenderEntity(IClientEntity *entity);

private:

	HFont font;
};

extern Visuals *g_Visuals;