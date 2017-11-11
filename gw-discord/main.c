#include <Windows.h>

#include "discord_integration.h"





BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
	DisableThreadLibraryCalls(hModule);
	switch(dwReason)
	{
		case DLL_PROCESS_ATTACH:
		{

		}
		break;
		case DLL_PROCESS_DETACH:
		{

		}
		break;
	}
	return TRUE;
}