// lolhack.cpp : Defines the entry point for the console application.
//

#include "lolhack.h"
#include "interfaces.h"
#include "aimbot.h"
#include "structs.h"
#include "wallhack.h"

#include <cmodel.h>

#include <sourcehook.h>
#include <sourcehook_impl.h>

using namespace SourceHook;

Impl::CSourceHookImpl g_SourceHook;
int g_PLID = 0;
ISourceHook *g_SHPtr = &g_SourceHook;

bool g_Stop = false;

SH_DECL_HOOK1_void(IBaseClientDLL, FrameStageNotify, SH_NOATTRIB, 0, ClientFrameStage_t);
SH_DECL_HOOK1_void(IVEngineClient, ClientCmd, SH_NOATTRIB, 0, const char *);

//SH_DECL_HOOK1(IVModelInfo, GetStudiomodel, SH_NOATTRIB, 0, studiohdr_t *, const model_t *);
SH_DECL_HOOK4_void(IVModelRender, DrawModelExecute, SH_NOATTRIB, 0, IMatRenderContext *, const DrawModelState_t &, const ModelRenderInfo_t &, matrix3x4_t *);

void Hook_ExecuteClientCmd(const char *msg)
{
	if (strcmp(msg, "bruh_stop") == 0)
	{
		g_Stop = true;
		RETURN_META(MRES_SUPERCEDE);
	}
	RETURN_META(MRES_IGNORED);
}
//HFont font;
void Hook_FrameStageNotify(ClientFrameStage_t stage)
{
	if (stage == ClientFrameStage_t::FRAME_NET_UPDATE_POSTDATAUPDATE_START)
	{
		Aimbot::Run();

		/*wchar_t buf[128] = L"hi i'm hack please no report!";

		if (!font)
		{
			font = vguiSurface->CreateFont();
			vguiSurface->SetFontGlyphSet(font, "Arial", 15, 100, 0, 0, 0);
		}

		vguiSurface->DrawSetTextFont(font);
		vguiSurface->DrawSetTextColor(255, 128, 128, 255);
		vguiSurface->DrawSetTextPos(10, 10);
		vguiSurface->DrawUnicodeString(L"hi i'm hack please no report!");*/
		//vguiSurface->DrawSetColor(255, 128, 0, 255);
		//vguiSurface->DrawFilledRect(5, 5, 50, 25);
	}
	RETURN_META(MRES_IGNORED);
}

void Hook_DrawModelExecute(IMatRenderContext *pRenderContext, const DrawModelState_t &state, const ModelRenderInfo_t &pInfo, matrix3x4_t *pCustomBoneToWorld)
{
	if (!g_Wallhack)
		g_Wallhack = new Wallhack();

	const auto mdl = pInfo.pModel;

	bool is_player = strstr(mdl->szName, "models/player") != nullptr;

	if (is_player) 
	{
		IMaterial* material = matSystem->FindMaterial(mdl->szName, TEXTURE_GROUP_MODEL);
		material->AlphaModulate(255.0f);
		material->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, true);
		material->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, true);
		modelRender->ForcedMaterialOverride(material);
		//g_Wallhack->Run();
	}

	RETURN_META(MRES_IGNORED);
}
/*
studiohdr_t	*Hook_GetStudiomodel(const model_t *mod)
{
	//studiohdr_t *std;
	//SH_CALL2(vmodelInfo, studiohdr_t, &IVModelInfo::GetStudiomodel)(mod);
	//RETURN_META_VALUE(MRES_IGNORED);
	return 0;
}
*/
void WINAPI Init(LPVOID dll_instance)
{
	if (!Interfaces::Init())
		FreeLibraryAndExitThread(static_cast<HMODULE>(dll_instance), 1);

	SH_ADD_HOOK(IBaseClientDLL, FrameStageNotify, client, SH_STATIC(Hook_FrameStageNotify), false);
	SH_ADD_VPHOOK(IVEngineClient, ClientCmd, engineClient, SH_STATIC(Hook_ExecuteClientCmd), false);

//	SH_ADD_VPHOOK(IVModelInfo, GetStudiomodel, vmodelInfo, SH_STATIC(Hook_GetStudiomodel), false);

	SH_ADD_VPHOOK(IVModelRender, DrawModelExecute, modelRender, SH_STATIC(Hook_DrawModelExecute), false);

	engineClient->ClientCmd_Unrestricted("echo hi i'm hack please no report!");

	while (!g_Stop)
	{
		Sleep(1000);
	}

	engineClient->ClientCmd_Unrestricted("echo k bruh i'm ded now");

	delete g_Wallhack;

	FreeLibraryAndExitThread(static_cast<HMODULE>(dll_instance), 1);
}

bool __stdcall DllMain(HINSTANCE dll_instance, DWORD call_reason, LPVOID reserved) {
	if (call_reason == DLL_PROCESS_ATTACH)
		CreateThread(0, 0, LPTHREAD_START_ROUTINE(Init), dll_instance, 0, 0);

	return true;
}