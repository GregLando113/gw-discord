#ifdef _GWDATA_H
#error "File included multiple times in obj."
#endif
#define _GWDATA_H

#include <wchar.h>

#define STATIC_ASSERT()

struct vec2f
{
    float x, y, z;
};

struct vec3f
{
    float x, y, z;
};

struct _gw_array
{
	void* data;
	unsigned cap;
	unsigned size;
	unsigned growth;
};

#define gw_array(T) struct { T* data; unsigned cap; unsigned size; unsigned growth; }

struct gwAgentContext;
struct gwMapContext;
struct gwWorldContext;
struct gwGadgetContext;
struct gwGuildContext;
struct gwGuildContext;
struct gwPartyContext;
struct gwTradeContext;

struct gwWorldContext 
{
    /* +h0000 */ wchar_t** name;
    /* +h0004 */ struct _gw_array message_buff;
    /* +h0014 */ char h0014[0x10];
    /* +h0024 */ struct _gw_array merchitems;
    /* +h0034 */ char h0034[0x48];
    /* +h007C */ struct _gw_array mapagents;
    /* +h008C */ char h008C[0x10];
    /* +h009C */ struct vec3f all_flag;
    /* +h00A8 */ char h00A8[0x4];
    /* +h00AC */ struct _gw_array attributes;
    /* +h00BC */ char h00BC[0x3FC];
    /* +h04B8 */ struct _gw_array h04B8;
    /* +h04C8 */ struct _gw_array h04C8;
    /* +h04D8 */ char h04D8[0x4];
    /* +h04DC */ struct _gw_array h04DC;
    /* +h04EC */ char h04EC[0x1C];
    /* +h0508 */ struct _gw_array partyeffects;
    /* +h0518 */ struct _gw_array h0518;
    /* +h0528 */ unsigned activequestid;
    /* +h052C */ struct _gw_array questlog;
    /* +h053C */ char h053C[0x38];
    /* +h0574 */ struct _gw_array h0574; // Seems like agent/item ids (only appears in outpost?)
    /* +h0584 */ struct _gw_array hero_flags;
    /* +h0594 */ struct _gw_array h0594;
    /* +h05A4 */ struct _gw_array h05A4; // Struct size = 0x20
    /* +h05B4 */ unsigned h05B4[2];
    /* +h05BC */ struct _gw_array h05BC;
    /* +h05CC */ struct _gw_array h05CC;
    /* +h05DC */ struct _gw_array h05DC;
    /* +h05EC */ struct _gw_array h05EC;
    /* +h05FC */ struct _gw_array h05FC;
    /* +h060C */ struct _gw_array h060C;
    /* +h061C */ unsigned h061C[4];
    /* +h062C */ struct _gw_array h062C;
    /* +h063C */ char h063C[0x44];
    /* +h0680 */ struct _gw_array h0680; // Struct size = 0x138 i think, has a vtable
    /* +h0690 */ unsigned salvagesessionid;
    /* +h0694 */ unsigned h0694[0xA];
    /* +h06BC */ struct _gw_array h06BC;
    /* +h06CC */ struct _gw_array h06CC;
    /* +h06DC */ unsigned h06DC;
    /* +h06E0 */ struct _gw_array h06E0;
    /* +h06F0 */ struct _gw_array skillbar;
    /* +h0700 */ struct _gw_array h0700;
    /* +h0710 */ struct _gw_array h0710;
    /* +h0720 */ struct _gw_array h0720;
    /* +h0730 */ struct _gw_array h0730;
    /* +h0740 */ unsigned experience;
    /* +h0744 */ unsigned experience_dupe;
    /* +h0748 */ unsigned currentkurzick;
    /* +h074C */ unsigned currentkurzick_dupe;
    /* +h0750 */ unsigned totalearnedkurzick;
    /* +h0754 */ unsigned totalearnedkurzick_dupe;
    /* +h0758 */ unsigned currentluxon;
    /* +h075C */ unsigned currentluxon_dupe;
    /* +h0760 */ unsigned totalearnedluxon;
    /* +h0764 */ unsigned totalearnedluxon_dupe;
    /* +h0768 */ unsigned currentimperial;
    /* +h076C */ unsigned currentimperial_dupe;
    /* +h0770 */ unsigned totalearnedimperial;
    /* +h0774 */ unsigned totalearnedimperial_dupe;
    /* +h0778 */ unsigned unkfaction4;
    /* +h077C */ unsigned unkfaction4_dupe;
    /* +h0780 */ unsigned unkfaction5;
    /* +h0784 */ unsigned unkfaction5_dupe;
    /* +h0788 */ unsigned level;
    /* +h078C */ unsigned level_dupe;
    /* +h0790 */ unsigned unkfaction6;
    /* +h0794 */ unsigned unkfaction6_dupe;
    /* +h0798 */ unsigned currentbalth;
    /* +h079C */ unsigned currentbalth_dupe;
    /* +h07A0 */ unsigned totalearnedbalth;
    /* +h07A4 */ unsigned totalearnedbalth_dupe;
    /* +h07A8 */ unsigned currentskillpoints;
    /* +h07AC */ unsigned currentskillpoints_dupe;
    /* +h07B0 */ unsigned totalearnedskillpoints;
    /* +h07B4 */ unsigned totalearnedskillpoints_dupe;
    /* +h07B8 */ unsigned maxkurzick;
    /* +h07BC */ unsigned maxluxon;
    /* +h07C0 */ unsigned maxbalth;
    /* +h07C4 */ unsigned maximperial;
    /* +h07C8 */ unsigned unk6;
    /* +h07CC */ struct _gw_array agentInfos;
    /* +h07DC */ struct _gw_array unk24_array;
    /* +h07EC */ struct _gw_array missionmapicons;
    /* +h07FC */ struct _gw_array npcs;
    /* +h080C */ struct _gw_array players;
    /* +h081C */ struct _gw_array titles;
    /* +h082C */ char pad9[0x20];
    /* +h084C */ unsigned foes_killed;
    /* +h0850 */ unsigned foes_to_kill;
    //... couple more arrays after this
};

struct gwCharContext 
{ // total: 0x03A8
    /* +h0000 */ struct _gw_array h0000;
    /* +h0010 */ char h0010[4];
    /* +h0014 */ struct _gw_array h0014;
    /* +h0024 */ char h0024[0x10];
    /* +h0034 */ struct _gw_array h0034;
    /* +h0044 */ struct _gw_array h0044;
    /* +h0054 */ char h0054[0x20];
    /* +h0074 */ wchar_t playername[20];
    /* +h009C */ char h009C[0x50];
    /* +h00EC */ struct _gw_array h00EC;
    /* +h00FC */ char h00FC[0x28];
    /* +h0124 */ unsigned h0124;
    /* +h0128 */ unsigned token2; // player unique id in the world
    /* +h012C */ unsigned mapidagain;
    /* +h0130 */ unsigned is_explorable;
    /* +h0134 */ char h0130[0x5C];
    /* +h0190 */ unsigned currentmapid_again;
    /* +h0194 */ char h0194[0x20];
	/* +h01B4 */ unsigned district;
	/* +h01B8 */ unsigned unk01B8;
    /* +h01BC */ unsigned currentmapid;
    /* +h01C0 */ unsigned currentmapid_dupe;
    /* +h01C4 */ char h01C4[0x8];
    /* +h01CC */ struct _gw_array ObserverMatchs;
    /* +h01DC */ char h001DC[0x14C];
    /* +h0328 */ wchar_t playeremail[64];
};


struct gwPlayerPartyMember 
{ // total: 0xC/12
    /* +h0000 */ unsigned loginnumber;
    /* +h0004 */ unsigned unk1;
    /* +h0008 */ unsigned state;
};

struct gwHeroPartyMember { // total: 0x18/24
    /* +h0000 */ unsigned agentid;
    /* +h0004 */ unsigned ownerplayerid;
    /* +h0008 */ unsigned heroid;
    /* +h000C */ unsigned h000C;
    /* +h0010 */ unsigned h0010;
    /* +h0014 */ unsigned level;
};

struct gwHenchmanPartyMember { // total: 0x34/52
    /* +h0000 */ unsigned agentid;
    /* +h0004 */ unsigned h0004[10];
    /* +h002C */ unsigned profession;
    /* +h0030 */ unsigned level;
};

struct gwPartyInfo 
{ // total: 0x44/68
    /* +h0000 */ unsigned h0000;
    /* +h0004 */ gw_array(struct gwPlayerPartyMember) players;
    /* +h0014 */ gw_array(struct gwHenchmanPartyMember) henchmen;
    /* +h0024 */ gw_array(struct gwHeroPartyMember) heroes;
    /* +h0034 */ gw_array(unsigned) others; // agent id of allies, minions, pets.
};

struct gwPartyContext 
{ // total: 0x58/88
    /* +h0000 */ unsigned h0000;
    /* +h0004 */ struct _gw_array h0004;
    /* +h0014 */ unsigned flag;
    /* +h0018 */ unsigned h0018[4];
    /* +h0028 */ unsigned InviteCount;
    /* +h002C */ char pad_0x002C[0x28];
    /* +h0054 */ struct gwPartyInfo *partyinfo;
};


struct gwGameContext 
{
    /* +h0000 */ void* unkcontext1;
    /* +h0004 */ void* unkcontext2;
    /* +h0008 */ struct gwAgentContext* agent; // Most functions that access are prefixed with Agent.
    /* +h000C */ void* unkcontext4;
    /* +h0010 */ void* nullcontext1;
    /* +h0014 */ struct gwMapContext* map; // Static object/collision data
    /* +h0018 */ void* text_parser; // TextParser context
    /* +h001C */ void* nullcontext2;
    /* +h0020 */ unsigned somenumber; // 0x30 for me at the moment.
    /* +h0024 */ void* unkcontext6;
    /* +h0028 */ void* unkcontext7;
    /* +h002C */ struct gwWorldContext* world; // Best name to fit it that I can think of.
    /* +h0030 */ void* unkcontext8;
    /* +h0034 */ void* nullcontext3;
    /* +h0038 */ struct gwGadgetContext* gadget;
    /* +h003C */ struct gwGuildContext* guild;
    /* +h0040 */ struct gwItemContext* items;
    /* +h0044 */ struct gwCharContext* character;
    /* +h0048 */ void* nullcontext4;
    /* +h004C */ struct gwPartyContext* party;
    /* +h0050 */ void* nullcontext5;
    /* +h0054 */ void* nullcontext6;
    /* +h0058 */ struct gwTradeContext* trade;
};

struct gwConstAreaInfo 
{ // total: 0x7C/120
    /* +h0000 */ unsigned h0000;
    /* +h0004 */ unsigned Continent;
    /* +h0008 */ unsigned Region;
    /* +h000C */ unsigned Type;
    /* +h0010 */ unsigned Flags;
    /* +h0014 */ unsigned thumbnailid;
	/* +h0018 */ unsigned unk1;
    /* +h001C */ unsigned maxpartysize;
    /* +h0020 */ unsigned h001C[8];
    /* +h0040 */ unsigned x; // icon position on map.
    /* +h0044 */ unsigned y;
    /* +h0048 */ unsigned h0048[11];
    /* +h0074 */ unsigned nameid;
    /* +h0078 */ unsigned descriptionid;
};


// MsgConn.cpp

struct rc4cipher
{
    unsigned x;
    unsigned y;
    unsigned char m[256];
};

typedef int __fastcall gwMsgHandler_t(void* msg);

struct gwCtoSMsgProtocol
{
    unsigned* arguments;
    unsigned numargs;
};

struct gwStoCMsgProtocol
{
    unsigned* arguments;
    unsigned numargs;
    gwMsgHandler_t* handler;
};


struct gwMsgConn
{
    char pad[0x08];
    struct __protocol
    {
        char pad[0x1C];
        gw_array(struct gwCtoSMsgProtocol) ctos;
		gw_array(struct gwStoCMsgProtocol) stoc;
    } *protocol;
    char pad2[0x94 - 0xC];
    struct rc4cipher outbound;
    struct rc4cipher inbound;
};

struct gwDistrictInfo
{
	unsigned unk1;
	unsigned unk2;
	unsigned unk3;
	unsigned language;
	int      region;
};


// initialization

int                     gw_initgamesrv(void);

// data
struct gwGameContext*   gw_gamecontext(void);
struct gwConstAreaInfo* gw_areainfo(unsigned mapid);
struct gwMsgConn*       gw_gamesrv(void);
struct gwDistrictInfo*  gw_districtinfo(void);

// commands
void                    gw_maptravel(unsigned mapid, unsigned region, unsigned language, unsigned district);
void                    gw_requestjoin(unsigned playerid);
gwMsgHandler_t*         gw_setmsghandler(struct gwMsgConn* conn, unsigned op, gwMsgHandler_t* handler);


typedef void __fastcall gwDecodeStringCallback_t(void* arg,wchar_t* str);
int                     gw_decodestringasync(void* encode, gwDecodeStringCallback_t* callback, void* arg);
void                    gw_encodestringid(unsigned id, unsigned short words[]);