#pragma once

#include "interfaces.h"

class Wallhack
{
public:
	Wallhack();
	~Wallhack();

	void Run();

private:
	IMaterial* materialRegular = nullptr;
	IMaterial* materialRegularIgnoreZ = nullptr;
	IMaterial* materialFlatIgnoreZ = nullptr;
	IMaterial* materialFlat = nullptr;
};

extern Wallhack *g_Wallhack;