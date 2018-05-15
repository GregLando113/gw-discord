#include "memory.h"


int scanfor(uintptr_t base, uintptr_t size, const char* sig, const char* mask, int offset, int deref, void** out)
{
	size += base;
	unsigned char first = (unsigned char)sig[0];
	for(; base < size; ++base)
	{
		if(*(unsigned char*)base != first)
		{
			continue;
		}
		int found = 1;
		const unsigned char* b = (const unsigned char*)base;
		const unsigned char* s = (const unsigned char*)sig;
		const char*          m = (const char*)mask;
		for(;*m;++b,++s,++m)
		{
			if(*m == '?')
			{
				continue;
			}
			if(*b != *s)
			{
				found = 0;
				break;
			}
		}
		if(found)
		{
			*out = deref ? *(void**)(base + offset) : (void*)(base + offset);
			return 1;
		}
	}
	return 0;
}