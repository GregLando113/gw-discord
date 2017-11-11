#ifdef _DISCORD_INTEGRATION_H
#error "Included multiple times in object."
#endif
#define _DISCORD_INTEGRATION_H



int  __stdcall gwdiscord_initialize(void* p);
void gwdiscord_deinitialize(void);