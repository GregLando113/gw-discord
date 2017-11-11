#include "discord_integration.h"

#include <discord-rpc.h>

#include "gwdata.h"
#include "base64.h"

#define DISCORD_APP_ID "378706083788881961"

char g_statebuffer[128];
char g_detailbuffer[128];
DiscordRichPresence g_presence = { 0 };



void __fastcall discord_decodestrcallback(void* data, wchar_t* str)
{
	char* b = g_statebuffer;
	for (; *str; ++b, ++str)
		*b = *str;
	Discord_UpdatePresence(&g_presence);
}


int getpartysize(struct gwGameContext* ctx)
{
	struct gwPartyInfo* p = ctx->party->partyinfo;
	return p->players.size + p->heroes.size + p->henchmen.size;
}

gwMsgHandler_t* oMsg33 = 0;
int __fastcall msg33_callback(void* cb)
{
	struct gwGameContext* ctx = gw_gamecontext();
	
	if (!ctx)
		goto end;

	unsigned mapid = ctx->character->currentmapid;
	struct gwAreaInfo* mapinfo = gw_areainfo(mapid);

	g_presence.partyMax = mapinfo->maxpartysize;
	g_presence.partySize = getpartysize(ctx);
	strcpy(g_detailbuffer, (ctx->character->is_explorable) ? "In Explorable" : "In Outpost");

	gw_decodestringasync(mapinfo->nameid, discord_decodestrcallback, NULL);

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

int  
gwdiscord_initialize(void)
{

	g_presence.state = g_statebuffer;
	g_presence.details = g_detailbuffer;

	DiscordEventHandlers handlers;
	memset(&handlers, 0, sizeof(handlers));
	handlers.ready        = discord_onready;
	handlers.errored      = discord_ondisconnected;
	handlers.disconnected = discord_onerrored;
	handlers.joinGame     = discord_onjoingame;

	gw_initgamesrv();
	Discord_Initialize(DISCORD_APP_ID, &handlers, 1, NULL);

	oMsg33 = gw_setmsghandler(gw_gamesrv(), 33, msg33_callback);
}

void 
gwdiscord_deinitialize(void)
{
	gw_setmsghandler(gw_gamesrv(), 33, oMsg33);
	Discord_Shutdown();
}