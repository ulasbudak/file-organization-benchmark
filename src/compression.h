#ifndef COMPRESSION_H
#define COMPRESSION_H

#include "sdk.h"
#include <zlib.h>
#include <stdio.h>

int sikistir(const unsigned char *src, unsigned long srcLen, unsigned char **dest, unsigned long *destLen);
int veri_ac(const unsigned char *src, unsigned long srcLen, unsigned char *dest, unsigned long *destLen);
char* dosyadan_oku_ve_ac(FILE* dosya);

#endif