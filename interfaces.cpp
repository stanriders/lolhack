#include "interfaces.h"

IBaseClientDLL *client = NULL;
IVEngineServer *engine = NULL;
IVEngineClient *engineClient = NULL;
IEngineTrace *traces = NULL;

IClientEntityList *entityList = NULL;

IVModelInfo *vmodelInfo = NULL;
IVModelRender *modelRender = NULL;

IMaterialSystem *matSystem = NULL;

ISurface *vguiSurface = NULL;

bool Interfaces::Init()
{
	CreateInterfaceFn EngineFactory = (CreateInterfaceFn)GetProcAddress(GetModuleHandle("engine.dll"), "CreateInterface");
	CreateInterfaceFn ClientFactory = (CreateInterfaceFn)GetProcAddress(GetModuleHandle("client.dll"), "CreateInterface");
	CreateInterfaceFn MatsysFactory = (CreateInterfaceFn)GetProcAddress(GetModuleHandle("materialsystem.dll"), "CreateInterface");
	CreateInterfaceFn VguiFactory = (CreateInterfaceFn)GetProcAddress(GetModuleHandle("vgui2.dll"), "CreateInterface");

	engine = (IVEngineServer *)((EngineFactory)(INTERFACEVERSION_VENGINESERVER, NULL));
	engineClient = (IVEngineClient *)((EngineFactory)("VEngineClient014", NULL)); // ssdk versions are outdated
	if (!engine || !engineClient)
	{
		return false;
	}

	client = (IBaseClientDLL *)((ClientFactory)("VClient018", NULL)); // ssdk versions are outdated
	if (!client)
	{
		return false;
	}

	entityList = (IClientEntityList *)((ClientFactory)(VCLIENTENTITYLIST_INTERFACE_VERSION, NULL));
	vmodelInfo = (IVModelInfo *)((ClientFactory)(VMODELINFO_CLIENT_INTERFACE_VERSION, NULL));

	modelRender = (IVModelRender *)((EngineFactory)(VENGINE_HUDMODEL_INTERFACE_VERSION, NULL));

	traces = (IEngineTrace *)((EngineFactory)(INTERFACEVERSION_ENGINETRACE_CLIENT, NULL));

	matSystem = (IMaterialSystem *)((MatsysFactory)("VMaterialSystem080", NULL));

	vguiSurface = (ISurface *)((VguiFactory)("VGUI_Surface031", NULL));

	return true;
}