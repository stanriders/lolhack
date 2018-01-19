#pragma once

#include "interfaces.h"

class Wallhack
{
public:
	Wallhack();
	~Wallhack();

	void Run(IMaterial *origMaterial, bool post);

private:
	IMaterial* materialFlatIgnoreZ = nullptr;
	IMaterial* materialFlat = nullptr;
};

extern Wallhack *g_Wallhack;