
#include "gwdata.h"
#include "memory.h"

#define GW_MODULE_BASE 0x00401000
#define GW_MODULE_SIZE 0x0049A000

static void(__fastcall    *g_msgsend)   (struct gwMsgConn* obj, unsigned size, void* buffer) = 0;
static struct gwMsgConn* (*g_getgamesrv)(void)                                               = 0;

int 
gw_initgamesrv()
{
	if(g_msgsend && g_getgamesrv)
	{
		return 1;
	}

	uintptr_t base;
	if(!scanfor(GW_MODULE_BASE, GW_MODULE_SIZE, 
		"\x8B\x45\x08\x89\x4D\xEC\x8B\x4D\x0C\x89\x45\xF4\x89\x55\xF0\x8B\x55\x10\x8D\x45\xE8", 
		"xxxxxxxxxxxxxxxxxxxxx", 
		0, 0, &base))
	{
		return 0;
	}

	g_getgamesrv = (void*)DECODE_REL(base + 0x2A);
	g_msgsend    = (void*)DECODE_REL(base + 0x36);

	return 1;
}

struct gwGameContext* 
gw_gamecontext(void)
{

	struct __ctx_base
	{
		struct 
		{
			char pad[0x18];
			struct gwGameContext* ctx;
		} *sub1;
	};
	static struct __ctx_base* ptr = 0;
	if(!ptr && !scanfor(GW_MODULE_BASE, GW_MODULE_SIZE,
					"\x85\xC0\x75\x0F\x8B\xCE",
					"xxxxxx", 
					-4, 
					1,&ptr))
	{
		return 0;
	}
	return (ptr && ptr->sub1 && ptr->sub1->ctx) ? ptr->sub1->ctx : 0;
}


struct gwAreaInfo*
gw_areainfo(unsigned mapid)
{
	static struct gwAreaInfo* ptr;
	if(!ptr && !scanfor(GW_MODULE_BASE, GW_MODULE_SIZE,
					"\x8B\xC6\xC1\xE0\x05\x2B\xC6\x5E\x8D", 
					"xxxxxxxxx", 
					11, 
					1, &ptr))
	{
		return 0;
	}
	return (ptr + mapid);
}

// commands
void
gw_maptravel(unsigned mapid, unsigned region, unsigned language, unsigned district)
{
	struct __msg00AB
	{
		unsigned op;
		unsigned mapid;
		unsigned region;
		unsigned district;
		unsigned language;
		unsigned unk4;
	};

	if(!gw_initgamesrv())
	{
		return;
	}

	struct __msg00AB msg = {
		.op       = 0x00AB,
		.mapid    = mapid,
		.region   = region,
		.district = district,
		.language = language,
		.unk4     = 0
	};

	g_msgsend(g_getgamesrv(), sizeof(msg), &msg);
}

void
gw_requestjoin(unsigned playerid)
{
	struct __msg009A
	{
		unsigned op;
		unsigned playerid;
	};

	if(!gw_initgamesrv() || !g_getgamesrv())
	{
		return;
	}
    struct __msg009A msg = {
		.op       = 0x009A,
		.playerid = playerid
	};

	g_msgsend(g_getgamesrv(), sizeof(msg), &msg);
}
gwMsgHandler_t*
gw_setmsghandler(struct gwMsgConn* conn, unsigned op, gwMsgHandler_t* handler)
{
	if(!conn || !conn->protocol)
	{
		return 0;
	}

	struct gw_array* stochandlers = &conn->protocol->stoc;
	struct gwStoCMsgProtocol* proto = gw_array_idxptr(struct gwStoCMsgProtocol*, *stochandlers, op);


	gwMsgHandler_t* ohandler = proto->handler;
	proto->handler = handler;


	return ohandler;
}

int
gw_decodestringasync(void* encode, gwDecodeStringCallback_t* callback, void* arg)
{
	static void (__fastcall *__decodestr)(void* end, gwDecodeStringCallback_t* cb, void* arg) = 0;
	if(!__decodestr && !scanfor(GW_MODULE_BASE,GW_MODULE_SIZE,
							"\x8D\x7C\x46\x02\x8B\xCE\x6A\x01\x8B\xD7",
							"xxxxxxxxxx",
							-0x88,
							0, &__decodestr))
	{
		return 0;
	}
	__decodestr(encode, callback, arg);
	return 1;
}

struct gwMsgConn*
gw_gamesrv(void)
{
	if (!g_getgamesrv)
	{
		return 0;
	}
	return g_getgamesrv();
}