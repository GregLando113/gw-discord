
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>


#ifdef __cplusplus
extern "C" {
#endif

#if _DEBUG
#include <stdio.h>
#define DBG(fmt,...) printf(fmt,__VA_ARGS__)
#else
#define DBG(fmt,...)
#endif

#define DLL_DIR_FIND  TEXT("bin\\")
#define D3D_CHAIN_DLL TEXT("d3d9-chain.dll")

struct IDirect3D9;
struct IDirect3DDevice9;
struct IDirect3DStateBlock9;
struct IDirect3DVertexDeclaration9;
struct IDirect3DVertexShader9;
struct IDirect3DPixelShader9;
struct IDirect3DResource9;
struct IDirect3DBaseTexture9;
struct IDirect3DTexture9;
struct IDirect3DVolumeTexture9;
struct IDirect3DCubeTexture9;
struct IDirect3DVertexBuffer9;
struct IDirect3DIndexBuffer9;
struct IDirect3DSurface9;
struct IDirect3DVolume9;
struct IDirect3DSwapChain9;
struct IDirect3DQuery9;

#define _FACD3D  0x876
#define MAKE_D3DHRESULT( code )  MAKE_HRESULT( 1, _FACD3D, code )

/*
* Direct3D Errors
*/
#define D3D_OK                              S_OK
#define D3DERR_NOTAVAILABLE                 MAKE_D3DHRESULT(2154)

typedef DWORD D3DCOLOR;


typedef struct IDirect3D9* WINAPI Direct3DCreate9_t(UINT);
typedef HRESULT WINAPI Direct3DCreate9Ex_t(UINT, struct IDirect3D9Ex**);

HMODULE g_d3d9dll;
unsigned g_dllcount = 0;
HMODULE g_d3dchaindll = 0;

static void LoadAllDlls(void)
{
	if (g_dllcount > 0)
		return;

	TCHAR path[MAX_PATH] = DLL_DIR_FIND;
	TCHAR* p = path + sizeof(DLL_DIR_FIND) - 1;
	WIN32_FIND_DATA fdata;
	strcpy(p, "*.dll");
	HANDLE fh = FindFirstFile(path, &fdata);
	if (fh != INVALID_HANDLE_VALUE) do
	{
		strcpy(p, fdata.cFileName);
		LoadLibrary(path);
		g_dllcount++;
		DBG("Loaded %s...\n", path);
	} while (FindNextFile(fh, &fdata));
	g_d3dchaindll = LoadLibrary(D3D_CHAIN_DLL);
}


static HANDLE LoadD3D9(void)
{
	if (g_d3d9dll == 0)
	{

		TCHAR path[MAX_PATH];
		GetSystemDirectory(path, ARRAYSIZE(path));

		unsigned i = 0;
		while (path[i])
		{
			++i;
		}

		TCHAR* dll = TEXT("\\d3d9.dll");

		unsigned j = 0;
		while (dll[j])
		{
			path[i++] = dll[j++];
		}

		path[i] = 0;
		g_d3d9dll = LoadLibrary(path);
	}

	return g_d3d9dll;
}

__declspec(dllexport)
struct IDirect3D9* WINAPI Direct3DCreate9(
	UINT SDKVersion
)
{
	static Direct3DCreate9_t* _imp_Direct3DCreate9 = NULL;
	HANDLE hD3D = LoadD3D9();

	LoadAllDlls();

	if ((g_d3dchaindll || hD3D) && !_imp_Direct3DCreate9) {
		_imp_Direct3DCreate9 = (Direct3DCreate9_t*)GetProcAddress(g_d3dchaindll ? g_d3dchaindll : hD3D, "Direct3DCreate9");
	}

	if (_imp_Direct3DCreate9)
		return _imp_Direct3DCreate9(SDKVersion);

	return NULL;
}


__declspec(dllexport)
HRESULT WINAPI Direct3DCreate9Ex(
	_In_  UINT         SDKVersion,
	_Out_ struct IDirect3D9Ex** ppD3D
)
{
	static Direct3DCreate9Ex_t* _imp_Direct3DCreate9Ex = NULL;
	HANDLE hD3D = LoadD3D9();

	LoadAllDlls();

	if ((g_d3dchaindll || hD3D) && !_imp_Direct3DCreate9Ex) {
		_imp_Direct3DCreate9Ex = (Direct3DCreate9Ex_t*)GetProcAddress(g_d3dchaindll ? g_d3dchaindll : hD3D, "Direct3DCreate9Ex");
	}

	if (_imp_Direct3DCreate9Ex)
		return _imp_Direct3DCreate9Ex(SDKVersion, ppD3D);

	return D3DERR_NOTAVAILABLE;
}

__declspec(dllexport)
int WINAPI D3DPERF_BeginEvent(D3DCOLOR col, LPCWSTR wszName)
{
	typedef int (WINAPI* BeginEvent_t)(D3DCOLOR, LPCWSTR);
	static BeginEvent_t _imp_BeginEvent = NULL;
	HANDLE hD3D = LoadD3D9();

	LoadAllDlls();

	if (hD3D && !_imp_BeginEvent)
		_imp_BeginEvent = (BeginEvent_t)GetProcAddress(g_d3dchaindll ? g_d3dchaindll : hD3D, "D3DPERF_BeginEvent");

	if (_imp_BeginEvent)
		return _imp_BeginEvent(col, wszName);

	return D3DERR_NOTAVAILABLE;
}

__declspec(dllexport)
int WINAPI D3DPERF_EndEvent(void)
{
	typedef int (WINAPI* EndEvent_t)(void);
	static EndEvent_t _imp_EndEvent = NULL;
	HANDLE hD3D = LoadD3D9();

	LoadAllDlls();

	if (hD3D && !_imp_EndEvent)
		_imp_EndEvent = (EndEvent_t)GetProcAddress(g_d3dchaindll ? g_d3dchaindll : hD3D, "D3DPERF_EndEvent");

	if (_imp_EndEvent)
		return _imp_EndEvent();

	return D3DERR_NOTAVAILABLE;
}

__declspec(dllexport)
void WINAPI D3DPERF_SetMarker(D3DCOLOR col, LPCWSTR wszName)
{
	typedef VOID(WINAPI* Direct3DSet_t)(D3DCOLOR, LPCWSTR);
	static Direct3DSet_t _imp_SetMarker = NULL;
	HANDLE hD3D = LoadD3D9();

	LoadAllDlls();

	if ((g_d3dchaindll || hD3D) && !_imp_SetMarker)
		_imp_SetMarker = (Direct3DSet_t)GetProcAddress(g_d3dchaindll ? g_d3dchaindll : hD3D, "D3DPERF_SetMarker");

	if (_imp_SetMarker)
		_imp_SetMarker(col, wszName);
}

__declspec(dllexport)
void WINAPI D3DPERF_SetRegion(D3DCOLOR col, LPCWSTR wszName)
{
	typedef VOID(WINAPI* Direct3DSet_t)(D3DCOLOR, LPCWSTR);
	static Direct3DSet_t _imp_SetRegion = NULL;
	HANDLE hD3D = LoadD3D9();

	LoadAllDlls();

	if ((g_d3dchaindll || hD3D) && !_imp_SetRegion)
		_imp_SetRegion = (Direct3DSet_t)GetProcAddress(g_d3dchaindll ? g_d3dchaindll : hD3D, "D3DPERF_SetRegion");

	if (_imp_SetRegion)
		_imp_SetRegion(col, wszName);
}

__declspec(dllexport)
BOOL WINAPI D3DPERF_QueryRepeatFrame(void)
{
	typedef BOOL(WINAPI* QueryRepeatFrame_t)(void);
	static QueryRepeatFrame_t _imp_QueryRepeatFrame = NULL;
	HANDLE hD3D = LoadD3D9();

	LoadAllDlls();

	if ((g_d3dchaindll || hD3D) && !_imp_QueryRepeatFrame)
		_imp_QueryRepeatFrame = (QueryRepeatFrame_t)GetProcAddress(g_d3dchaindll ? g_d3dchaindll : hD3D, "D3DPERF_QueryRepeatFrame");

	if (_imp_QueryRepeatFrame)
		return _imp_QueryRepeatFrame();

	return FALSE;
}

__declspec(dllexport)
void WINAPI D3DPERF_SetOptions(DWORD dwOptions)
{
	typedef void (WINAPI* SetOptions_t)(DWORD);
	static SetOptions_t _imp_SetOptions = NULL;
	HANDLE hD3D = LoadD3D9();

	if ((g_d3dchaindll || hD3D) && !_imp_SetOptions)
		_imp_SetOptions = (SetOptions_t)GetProcAddress(g_d3dchaindll ? g_d3dchaindll : hD3D, "D3DPERF_SetOptions");

	if (_imp_SetOptions)
		_imp_SetOptions(dwOptions);
}

__declspec(dllexport)
DWORD WINAPI D3DPERF_GetStatus(void)
{
	typedef DWORD(WINAPI* GetStatus_t)(void);
	static GetStatus_t _imp_GetStatus = NULL;
	HANDLE hD3D = LoadD3D9();

	if ((g_d3dchaindll || hD3D) && !_imp_GetStatus)
		_imp_GetStatus = (GetStatus_t)GetProcAddress(g_d3dchaindll ? g_d3dchaindll : hD3D, "D3DPERF_GetStatus");

	if (_imp_GetStatus)
		return _imp_GetStatus();

	return 0;
}



#if _DEBUG
BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		AllocConsole();
		freopen("CONOUT$", "w", stdout);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		FreeConsole();
	}
	return TRUE;
}
#endif

#ifdef __cplusplus
}
#endif