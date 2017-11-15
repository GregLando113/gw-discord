#ifdef _BASE64_H
#error "File included multiple times in obj"
#endif
#define _BASE64_H




int b64_enc(void* in, unsigned size, char* out);
int b64_dec(char* in, void* out);