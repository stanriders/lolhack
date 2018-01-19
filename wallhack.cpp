#include <fstream>
#include "wallhack.h"

Wallhack *g_Wallhack;

Wallhack::Wallhack()
{
	std::ofstream("csgo\\materials\\simple_ignorez.vmt") << R"#("VertexLitGeneric"
{
  "$basetexture" "vgui/white_additive"
  "$ignorez"      "1"
  "$envmap"       ""
  "$nofog"        "1"
  "$model"        "1"
  "$nocull"       "0"
  "$selfillum"    "1"
  "$halflambert"  "1"
  "$znearer"      "0"
  "$flat"         "1"
}
)#";
	std::ofstream("csgo\\materials\\simple.vmt") << R"#("VertexLitGeneric"
{
  "$basetexture" "vgui/white_additive"
  "$ignorez"      "0"
  "$envmap"       ""
  "$nofog"        "1"
  "$model"        "1"
  "$nocull"       "0"
  "$selfillum"    "1"
  "$halflambert"  "1"
  "$znearer"      "0"
  "$flat"         "1"
}
)#";

	materialFlatIgnoreZ = g_matSystem->FindMaterial("simple_ignorez", TEXTURE_GROUP_MODEL);
	materialFlat = g_matSystem->FindMaterial("simple", TEXTURE_GROUP_MODEL);
}


Wallhack::~Wallhack()
{
}

void Wallhack::Run(IMaterial *origMaterial, bool post)
{
	if (!g_WallhackEnabled)
		return;

	if (materialFlat && materialFlatIgnoreZ)
	{
		if (!post)
		{
			origMaterial = materialFlatIgnoreZ;
			origMaterial->ColorModulate(200, 128, 0);
		}
		else
		{
			origMaterial = materialFlat;
			origMaterial->ColorModulate(0, 128, 200);
		}
		origMaterial->AlphaModulate(255.0f);
		g_modelRender->ForcedMaterialOverride(origMaterial);
	}
}