#include "discord_integration.h"

#include <discord-rpc.h>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "gwdata.h"
#include "base64.h"

#define DISCORD_APP_ID "378706083788881961"

char g_statebuffer[128];
char g_detailbuffer[128];
char g_largeimgkeybuffer[128];
DiscordRichPresence g_presence = { 0 };

const char* large_imgs[27] = 
{
	"region_kryta",
	"region_maguuma",
	"region_ascalon",
	"region_shiverpeaks",
	"region_ha",
	"region_crystaldesert",
	"region_fow",
	"region_presearing",
	"region_kaineng",
	"region_kurz",
	"region_lux",
	"region_shingjea",
	"region_kourna",
	"region_vabbi",
	"region_deso",
	"region_istan",
	"region_torment",
	"region_tarnished",
	"region_depths",
	"region_farshivs",
	"region_charrhomelands",
	"region_battleisles",
	"region_battlejahai",
	"region_flightnorth",
	"region_tenguaccords",
	"region_whitemantle",
	"region_swat"
};



void __fastcall 
discord_decodestrcallback(void* data, wchar_t* str)
{
	memset(g_detailbuffer, 0, 128);
	char* b = g_detailbuffer;
	for (; *str; ++b, ++str)
		*b = *str;
	Discord_UpdatePresence(&g_presence);
}


int 
getpartysize(struct gwGameContext* ctx)
{
	struct gwPartyInfo* p = ctx->party->partyinfo;
	return p->players.size + p->heroes.size + p->henchmen.size;
}

gwMsgHandler_t* oMsg33 = 0;
int __fastcall 
msg33_callback(void* cb)
{
	struct gwGameContext* ctx = gw_gamecontext();
	
	if (!ctx)
		goto end;

	unsigned mapid = ctx->character->currentmapid;
	struct gwAreaInfo* mapinfo = gw_areainfo(mapid);

	g_presence.partyMax = mapinfo->maxpartysize;
	//g_presence.partySize = getpartysize(ctx);
	strcpy(g_statebuffer, (ctx->character->is_explorable) ? "In Explorable" : "In Outpost");
	strcpy(g_largeimgkeybuffer, large_imgs[mapinfo->Region > 27 ? 0 : mapinfo->Region]);

	unsigned short encstr[4] = { 0 };
	gw_encodestringid(mapinfo->nameid, encstr);
	gw_decodestringasync(encstr, discord_decodestrcallback, NULL);

end:
	return oMsg33(cb);
}

void 
discord_onready(void)
{

}

void 
discord_ondisconnected(int errcode, const char* msg)
{

}

void 
discord_onerrored(int errcode, const char* msg)
{

}

void 
discord_onjoingame(const char* joinsecret)
{

}

int __stdcall
gwdiscord_initialize(void* p)
{

	g_presence.state = g_statebuffer;
	g_presence.details = g_detailbuffer;
	g_presence.largeImageKey = g_largeimgkeybuffer;

	DiscordEventHandlers handlers;
	memset(&handlers, 0, sizeof(handlers));
	handlers.ready        = discord_onready;
	handlers.errored      = discord_ondisconnected;
	handlers.disconnected = discord_onerrored;
	handlers.joinGame     = discord_onjoingame;

	gw_initgamesrv();

	while (!gw_gamesrv())
	{
		Sleep(32);
	}

	Discord_Initialize(DISCORD_APP_ID, &handlers, 1, NULL);

	oMsg33 = gw_setmsghandler(gw_gamesrv(), 23, msg33_callback);
}

void 
gwdiscord_deinitialize(void)
{
	gw_setmsghandler(gw_gamesrv(), 23, oMsg33);
	Discord_Shutdown();
}