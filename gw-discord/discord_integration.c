#include "discord_integration.h"

#include <discord-rpc.h>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <stdio.h>

#include "gwdata.h"
#include "base64.h"
#include "sha1.h"

#define DISCORD_APP_ID "378706083788881961"

char g_statebuffer[128];
char g_detailbuffer[128];
char g_largeimgkeybuffer[32];
char g_partyid[128];
char g_joinsecret[128];
DiscordRichPresence g_presence = { 0 };

unsigned g_partylocalid = 0;
unsigned g_playerlocalid = 0;
unsigned g_myplayerlocalid = 0;



enum
{
	kState_Active = 1 << 0
};

unsigned g_state = 0;


struct discord_joinsecret
{
	unsigned short mapid;
	unsigned char  district;
	char           region;
	unsigned char  language;
	unsigned char  playerid;
	char           partyid[20];
};

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

const char* map_regions[] = 
{
	"America",
	"Asia Korean",
	"Europe",
	"Asia Chinese",
	"Asia Japanese"
};

const char* map_languages[] =
{
	"English",
	"Unknown",
	"French",
	"German",
	"Italian",
	"Spanish",
	"Unknown",
	"Unknown",
	"Unknown",
	"Polish",
	"Russian"
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
	if (!p) return 0;
	return p->players.size + p->heroes.size + p->henchmen.size;
}

gwMsgHandler_t* oMsg23 = 0;
int __fastcall 
msg23_callback(void* cb) // set controlled character (used for map change load)
{
	struct __msg33
	{
		unsigned op;
		unsigned agid;
		unsigned unk1;
	} *p = cb;
	struct gwGameContext* ctx = gw_gamecontext();
	
	if (!ctx)
		goto end;

	unsigned mapid = ctx->character->currentmapid;
	struct gwConstAreaInfo* mapinfo = gw_areainfo(mapid);

	// get playerid for the instance
	struct gwAgent* ag = gw_getagentbyid(p->agid);
	if (ag)
	{
		g_myplayerlocalid = ag->LoginNumber;
	}
	g_presence.partyMax = mapinfo->maxpartysize;

	if (ctx->character->is_explorable)
	{
		strcpy(g_statebuffer, "In Explorable");
	}
	else
	{
		struct gwDistrictInfo* disinfo = gw_districtinfo();
		switch(disinfo->region)
		{
		case -2:
			sprintf_s(g_statebuffer, 128, "International - District %d", ctx->character->district);
			break;
		case 1:
		case 3:
		case 4:
			sprintf_s(g_statebuffer, 128, "%s - District %d", map_regions[disinfo->region], ctx->character->district);
			break;
		default:
			sprintf_s(g_statebuffer, 128, "%s %s - District %d", map_regions[disinfo->region], map_languages[disinfo->language], ctx->character->district);
			break;
		}		
	}


	strcpy(g_largeimgkeybuffer, large_imgs[mapinfo->Region > 27 ? 0 : mapinfo->Region]);

	unsigned short encstr[4] = { 0 };
	gw_encodestringid(mapinfo->nameid, encstr);
	gw_decodestringasync(encstr, discord_decodestrcallback, NULL);

	if (g_playerlocalid)
	{
		gw_requestjoin(g_playerlocalid);
		g_playerlocalid = 0;
	}

end:
	return oMsg23(cb);
}


gwMsgHandler_t* oMsg462 = 0;
int __fastcall
msg462_callback(void* vp) // party search create
{
	struct __msg462
	{
		unsigned op;
		unsigned localid;
		unsigned district;
		unsigned matched;
		unsigned partysize;
		unsigned heroes;
		unsigned type;
		unsigned hardmode;
		wchar_t  msg[32];
		wchar_t name[20];
	} *p = vp;

	struct gwGameContext* ctx = gw_gamecontext();

	// check player of party search is controlled char
	if (wcscmp(p->name, ctx->character->playername))
	{
		goto end;
	}

	// make partyid
	char partyhash[20];
	SHA1(partyhash, p->msg, sizeof(wchar_t) * (32 + 20));
	memset(g_partyid, 0, 128);
	b64_enc(partyhash, 20, g_partyid);

	// make joinsecret
	struct gwDistrictInfo* dinfo = gw_districtinfo();
	struct discord_joinsecret secret =
	{
		.mapid = ctx->character->currentmapid,
		.district = ctx->character->district,
		.region   = dinfo->region,
		.language = dinfo->language,
		.playerid = g_myplayerlocalid
	};
	memcpy(secret.partyid, partyhash, 20);
	memset(g_joinsecret, 0, 128);
	b64_enc(&secret, sizeof(struct discord_joinsecret), g_joinsecret);

	// update discord with party search description
	struct gwDistrictInfo* disinfo = gw_districtinfo();
	switch (disinfo->region)
	{
	case -2:
		sprintf_s(g_statebuffer, 128, "International - District %d\n%S", ctx->character->district, p->msg);
		break;
	case 1:
	case 3:
	case 4:
		sprintf_s(g_statebuffer, 128, "%s - District %d\n%S", map_regions[disinfo->region], ctx->character->district, p->msg);
		break;
	default:
		sprintf_s(g_statebuffer, 128, "%s %s - District %d\n%S", map_regions[disinfo->region], map_languages[disinfo->language], ctx->character->district, p->msg);
		break;
	}

	g_partylocalid = p->localid;
	g_presence.partyId = g_partyid;
	g_presence.joinSecret = g_joinsecret;
	g_presence.instance = 1;
	g_presence.partySize = p->partysize;
	Discord_UpdatePresence(&g_presence);
end:
	return oMsg462(vp);
}

gwMsgHandler_t* oMsg464 = 0;
int __fastcall
msg464_callback(void* vp) // party search remove
{
	struct __msg464
	{
		unsigned op;
		unsigned localid;
	} *p = (struct __msg464*)vp;

	// msg must be about players party
	if (p->localid != g_partylocalid)
	{
		goto end;
	}
	g_partylocalid = 0;

	// update discord without party search description
	struct gwGameContext*  ctx = gw_gamecontext();
	struct gwDistrictInfo* disinfo = gw_districtinfo();
	switch (disinfo->region)
	{
	case -2:
		sprintf_s(g_statebuffer, 128, "International - District %d", ctx->character->district);
		break;
	case 1:
	case 3:
	case 4:
		sprintf_s(g_statebuffer, 128, "%s - District %d", map_regions[disinfo->region], ctx->character->district);
		break;
	default:
		sprintf_s(g_statebuffer, 128, "%s %s - District %d", map_regions[disinfo->region], map_languages[disinfo->language], ctx->character->district);
		break;
	}

	// clear party from discord
	g_presence.partyId = 0;
	g_presence.joinSecret = 0;
	g_presence.partySize = 0;
	Discord_UpdatePresence(&g_presence);
end:
	return oMsg464(vp);
}

gwMsgHandler_t* oMsg164 = 0;
int __fastcall msg164_callback(void* vp) // set player party size
{
	struct __msg164
	{
		unsigned op;
		unsigned playerid;
		unsigned size;
	} *p = vp;

	// we only care about our party
	if (p->playerid != g_partylocalid)
	{
		goto end;
	}

	g_presence.partySize = p->size;

	Discord_UpdatePresence(&g_presence);
end:
	return oMsg164(vp);
}

gwMsgHandler_t* oMsg429 = 0;
int __fastcall msg429_callback(void* vp) // instance change error
{
	enum
	{
		kInstError_CannotEnterOutpost = 0x35,
		kInstError_InActiveDistrict = 0x3c
	};

	struct __msg429
	{
		unsigned op;
		unsigned errcode;
	} *p = vp;


	switch (p->errcode)
	{
	case kInstError_CannotEnterOutpost:
	{
		if (g_playerlocalid)
		{
			g_playerlocalid = 0;
			g_presence.partyId = 0;
		}
	}
	break;
	case kInstError_InActiveDistrict:
	{
		if (g_playerlocalid)
		{
			gw_requestjoin(g_playerlocalid);
			g_playerlocalid = 0;
		}
	}
	break;
	}

end:
	return oMsg429(vp);
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
	struct discord_joinsecret s;
	b64_dec(joinsecret, &s);
	g_playerlocalid = s.playerid;
	b64_enc(s.partyid, 20, g_partyid);
	g_presence.partyId = g_partyid;
	Discord_UpdatePresence(&g_presence);
	gw_maptravel(s.mapid, s.region, s.language, s.district);
}

int __stdcall
gwdiscord_initialize(void* p)
{
	g_presence.instance = 1;
	g_presence.state = g_statebuffer;
	g_presence.details = g_detailbuffer;
	g_presence.largeImageKey = g_largeimgkeybuffer;
	g_state |= kState_Active;

	DiscordEventHandlers handlers;
	memset(&handlers, 0, sizeof(handlers));
	handlers.ready        = discord_onready;
	handlers.errored      = discord_onerrored;
	handlers.disconnected = discord_ondisconnected;
	handlers.joinGame     = discord_onjoingame;

	gw_initgamesrv();

	while (!gw_gamesrv())
	{
		Sleep(32);
	}

	Discord_Initialize(DISCORD_APP_ID, &handlers, 1, NULL);

	oMsg23  = gw_setmsghandler(gw_gamesrv(), 23,  msg23_callback);
	oMsg462 = gw_setmsghandler(gw_gamesrv(), 462, msg462_callback);
	oMsg464 = gw_setmsghandler(gw_gamesrv(), 464, msg464_callback);
	oMsg164 = gw_setmsghandler(gw_gamesrv(), 164, msg164_callback);
	oMsg429 = gw_setmsghandler(gw_gamesrv(), 429, msg429_callback);

	while (g_state & kState_Active)
	{
		Discord_RunCallbacks();
		Sleep(32);
	}
}

void 
gwdiscord_deinitialize(void)
{
	gw_setmsghandler(gw_gamesrv(), 23,  oMsg23);
	gw_setmsghandler(gw_gamesrv(), 462, oMsg462);
	gw_setmsghandler(gw_gamesrv(), 464, oMsg464);
	gw_setmsghandler(gw_gamesrv(), 164, oMsg164);
	gw_setmsghandler(gw_gamesrv(), 429, oMsg429);
	g_state &= ~kState_Active;
	Discord_Shutdown();
}