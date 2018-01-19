#include "hooks.h"
#include "aimbot.h"
#include "structs.h"
#include "wallhack.h"
#include "lolhack.h"
#include "visuals.h"

#include <sourcehook.h>
#include <sourcehook_impl.h>

SH_DECL_HOOK1_void(IBaseClientDLL, FrameStageNotify, SH_NOATTRIB, 0, ClientFrameStage_t);
SH_DECL_HOOK1_void(IVEngineClient, ClientCmd, SH_NOATTRIB, 0, const char *);
//SH_DECL_HOOK1(IVModelInfo, GetStudiomodel, SH_NOATTRIB, 0, studiohdr_t *, const model_t *);
SH_DECL_HOOK4_void(IVModelRender, DrawModelExecute, SH_NOATTRIB, 0, IMatRenderContext *, const DrawModelState_t &, const ModelRenderInfo_t &, matrix3x4_t *);

SH_DECL_HOOK3_void(IPanel, PaintTraverse, SH_NOATTRIB, 0, VPANEL, bool, bool);

/*
studiohdr_t	*Hook_GetStudiomodel(const model_t *mod)
{
//studiohdr_t *std;
//SH_CALL2(vmodelInfo, studiohdr_t, &IVModelInfo::GetStudiomodel)(mod);
//RETURN_META_VALUE(MRES_IGNORED);
return 0;
}
*/
void Hook_ExecuteClientCmd(const char *msg)
{
	if (strcmp(msg, "bruh_stop") == 0)
	{
		g_Stop = true;
		RETURN_META(MRES_SUPERCEDE);
	}
	RETURN_META(MRES_IGNORED);
}

void Hook_FrameStageNotify(ClientFrameStage_t stage)
{
	if (stage == ClientFrameStage_t::FRAME_NET_UPDATE_POSTDATAUPDATE_START)
	{
		Aimbot::Run();
	}
	RETURN_META(MRES_IGNORED);
}

void Hook_DrawModelExecute(IMatRenderContext *pRenderContext, const DrawModelState_t &state, const ModelRenderInfo_t &pInfo, matrix3x4_t *pCustomBoneToWorld)
{
	if (!g_Wallhack)
		g_Wallhack = new Wallhack();

	const auto mdl = pInfo.pModel;
	if (strstr(mdl->szName, "models/player") != nullptr)
	{
		IMaterial* material = g_matSystem->FindMaterial(mdl->szName, TEXTURE_GROUP_MODEL);
		g_Wallhack->Run(material, false);
	}

	RETURN_META(MRES_IGNORED);
}
void Hook_DrawModelExecute_Post(IMatRenderContext *pRenderContext, const DrawModelState_t &state, const ModelRenderInfo_t &pInfo, matrix3x4_t *pCustomBoneToWorld)
{
	if (!g_Wallhack)
		g_Wallhack = new Wallhack();

	const auto mdl = pInfo.pModel;
	if (strstr(mdl->szName, "models/player") != nullptr)
	{
		IMaterial* material = g_matSystem->FindMaterial(mdl->szName, TEXTURE_GROUP_MODEL);
		g_Wallhack->Run(material, true);
	}

	RETURN_META(MRES_IGNORED);
}
void Hook_PaintTraverse_Post(VPANEL vguiPanel, bool forceRepaint, bool allowForce)
{
	static auto panelId = vgui::VPANEL{ 0 };
	if (!panelId) {
		const auto panelName = g_vguiPanel->GetName(vguiPanel);
		if (!strcmp(panelName, "FocusOverlayPanel")) {
			panelId = vguiPanel;
		}
	}
	else if (panelId == vguiPanel)
	{
		g_Visuals->RenderCrosshair();

		for (int i = 1; i < g_engineClient->GetMaxClients(); ++i)
		{
			IClientEntity* player = g_entityList->GetClientEntity(i);

			if (player)
			{
				g_Visuals->RenderEntity(player);
			}
		}
		
	}
	RETURN_META(MRES_IGNORED);
}
void Hooks::Init()
{
	SH_ADD_HOOK(IBaseClientDLL, FrameStageNotify, g_client, SH_STATIC(Hook_FrameStageNotify), false);
	SH_ADD_VPHOOK(IVEngineClient, ClientCmd, g_engineClient, SH_STATIC(Hook_ExecuteClientCmd), false);
	//	SH_ADD_VPHOOK(IVModelInfo, GetStudiomodel, g_vmodelInfo, SH_STATIC(Hook_GetStudiomodel), false);
	SH_ADD_HOOK(IVModelRender, DrawModelExecute, g_modelRender, SH_STATIC(Hook_DrawModelExecute), false);
	SH_ADD_HOOK(IVModelRender, DrawModelExecute, g_modelRender, SH_STATIC(Hook_DrawModelExecute_Post), true);

	SH_ADD_HOOK(IPanel, PaintTraverse, g_vguiPanel, SH_STATIC(Hook_PaintTraverse_Post), true);
}

void Hooks::RemoveHooks()
{
	SH_REMOVE_HOOK(IBaseClientDLL, FrameStageNotify, g_client, SH_STATIC(Hook_FrameStageNotify), false);
	SH_REMOVE_HOOK(IVEngineClient, ClientCmd, g_engineClient, SH_STATIC(Hook_ExecuteClientCmd), false);
	SH_REMOVE_HOOK(IVModelRender, DrawModelExecute, g_modelRender, SH_STATIC(Hook_DrawModelExecute), false);
	SH_REMOVE_HOOK(IVModelRender, DrawModelExecute, g_modelRender, SH_STATIC(Hook_DrawModelExecute_Post), true);

	delete g_Wallhack;
}