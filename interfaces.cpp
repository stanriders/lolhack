#include "interfaces.h"

IBaseClientDLL *g_client = NULL;
IVEngineServer *g_engine = NULL;
IVEngineClient *g_engineClient = NULL;
IEngineTrace *g_traces = NULL;

IClientEntityList *g_entityList = NULL;

IVModelInfo *g_vmodelInfo = NULL;
IVModelRender *g_modelRender = NULL;

IMaterialSystem *g_matSystem = NULL;

ISurface *g_vguiSurface = NULL;
IPanel *g_vguiPanel = NULL;

bool Interfaces::Init()
{
	CreateInterfaceFn EngineFactory = (CreateInterfaceFn)GetProcAddress(GetModuleHandle("engine.dll"), "CreateInterface");
	CreateInterfaceFn ClientFactory = (CreateInterfaceFn)GetProcAddress(GetModuleHandle("client.dll"), "CreateInterface");
	CreateInterfaceFn MatsysFactory = (CreateInterfaceFn)GetProcAddress(GetModuleHandle("materialsystem.dll"), "CreateInterface");
	CreateInterfaceFn VguiSurfaceFactory = (CreateInterfaceFn)GetProcAddress(GetModuleHandle("vguimatsurface.dll"), "CreateInterface");
	CreateInterfaceFn VguiFactory = (CreateInterfaceFn)GetProcAddress(GetModuleHandle("vgui2.dll"), "CreateInterface");

	g_engine = (IVEngineServer *)((EngineFactory)(INTERFACEVERSION_VENGINESERVER, NULL));
	g_engineClient = (IVEngineClient *)((EngineFactory)("VEngineClient014", NULL)); // ssdk versions are outdated
	if (!g_engine || !g_engineClient)
	{
		return false;
	}

	g_client = (IBaseClientDLL *)((ClientFactory)("VClient018", NULL)); // ssdk versions are outdated
	if (!g_client)
	{
		return false;
	}

	g_entityList = (IClientEntityList *)((ClientFactory)(VCLIENTENTITYLIST_INTERFACE_VERSION, NULL));
	g_vmodelInfo = (IVModelInfo *)((ClientFactory)(VMODELINFO_CLIENT_INTERFACE_VERSION, NULL));

	g_modelRender = (IVModelRender *)((EngineFactory)(VENGINE_HUDMODEL_INTERFACE_VERSION, NULL));

	g_traces = (IEngineTrace *)((EngineFactory)(INTERFACEVERSION_ENGINETRACE_CLIENT, NULL));

	g_matSystem = (IMaterialSystem *)((MatsysFactory)("VMaterialSystem080", NULL));

	g_vguiPanel = (IPanel *)((VguiFactory)("VGUI_Panel009", NULL));
	g_vguiSurface = (ISurface *)((VguiSurfaceFactory)("VGUI_Surface031", NULL));

	return true;
}