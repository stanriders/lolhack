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

	materialRegularIgnoreZ = matSystem->FindMaterial("simple_ignorez", TEXTURE_GROUP_MODEL);
}


Wallhack::~Wallhack()
{
}

void Wallhack::Run()
{
	if (materialFlatIgnoreZ)
	{
		IMaterial* material = materialFlatIgnoreZ;
		material->AlphaModulate(255.0f);
		material->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, true);
		material->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, true);
		modelRender->ForcedMaterialOverride(material);
	}
}