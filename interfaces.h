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

//engine
extern IBaseClientDLL *client;
extern IVEngineServer *engine;
extern IVEngineClient *engineClient;
extern IEngineTrace *traces;
extern IClientEntityList *entityList;

//models
extern IVModelInfo *vmodelInfo;
extern IVModelRender *modelRender;

//materials
extern IMaterialSystem *matSystem;

//vgui
using namespace vgui;
extern ISurface *vguiSurface;

class Interfaces
{
public:
	static bool Init();
};