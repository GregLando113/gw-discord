#ifdef _MEMORY_H
#error "File included multiple times in obj."
#endif
#define _MEMORY_H


#include <stdint.h>


#define DECODE_REL(addr)     ((uintptr_t)((addr) + 4) + *(uint32_t*)(addr))
#define ENCODE_REL(src, dst) ((dst) - ((src) + 4))


int scanfor(uintptr_t base, uintptr_t size, const char* sig, const char* mask, int offset, int deref, void** out);