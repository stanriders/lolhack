// lolhack.cpp : Defines the entry point for the console application.
//

#include "lolhack.h"
#include "interfaces.h"
#include "hooks.h"

#include <cmodel.h>

#include <sourcehook.h>
#include <sourcehook_impl.h>

using namespace SourceHook;

Impl::CSourceHookImpl g_SourceHook;
int g_PLID = 0;
ISourceHook *g_SHPtr = &g_SourceHook;

bool g_Stop = false;

void WINAPI Init(LPVOID dll_instance)
{
	if (!Interfaces::Init())
		FreeLibraryAndExitThread(static_cast<HMODULE>(dll_instance), 1);

	Hooks hooks;
	hooks.Init();

	g_engineClient->ClientCmd_Unrestricted("echo hi i'm hack please no report!");

	while (!g_Stop)
	{
		Sleep(1000);
	}

	g_engineClient->ClientCmd_Unrestricted("echo k bruh i'm ded now");

	hooks.RemoveHooks();

	FreeLibraryAndExitThread(static_cast<HMODULE>(dll_instance), 1);
}

bool __stdcall DllMain(HINSTANCE dll_instance, DWORD call_reason, LPVOID reserved) {
	if (call_reason == DLL_PROCESS_ATTACH)
		CreateThread(0, 0, LPTHREAD_START_ROUTINE(Init), dll_instance, 0, 0);

	return true;
}