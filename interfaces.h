#pragma once

#include <stdio.h>
#include <windows.h>

#include <eiface.h>
#include <cdll_int.h>
#include <icliententitylist.h>
#include <icliententity.h>
#include <client_class.h>
#include <iclientunknown.h>
#include <ienginetrace.h>
#include <materialsystem/imaterialsystem.h>
#include <vgui/isurface.h>
#include <vgui/ipanel.h>

#include "settings.h"

//engine
extern IBaseClientDLL *g_client;
extern IVEngineServer *g_engine;
extern IVEngineClient *g_engineClient;
extern IEngineTrace *g_traces;
extern IClientEntityList *g_entityList;

//models
extern IVModelInfo *g_vmodelInfo;
extern IVModelRender *g_modelRender;

//materials
extern IMaterialSystem *g_matSystem;

//vgui
using namespace vgui;
extern ISurface *g_vguiSurface;
extern IPanel *g_vguiPanel;

class Interfaces
{
public:
	static bool Init();
};