#include <Windows.h>

#include "discord_integration.h"

#include <stdio.h>



BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
	DisableThreadLibraryCalls(hModule);
	switch(dwReason)
	{
		case DLL_PROCESS_ATTACH:
		{
			CreateThread(0, 0, (LPTHREAD_START_ROUTINE)gwdiscord_initialize, 0, 0, 0);
		}
		break;
		case DLL_PROCESS_DETACH:
		{
			gwdiscord_deinitialize();
		}
		break;
	}
	return TRUE;
}