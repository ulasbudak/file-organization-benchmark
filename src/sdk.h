#ifndef SDK_H
#define SDK_H

#include <stdio.h>   // FILE, fopen, printf, ftell, fwrite için
#include <stdlib.h>  // NULL, free, malloc için
#include <string.h>  // strcmp, strlen, memcmp için
#include <stdint.h>  // uint32_t gibi tipler için

#include <openssl/evp.h>

char* satiroku(FILE* file);
char* process(char satir[]);
void dosya_yaz(FILE* file, char satir[]);
int tekrar_kontrol(FILE* file, char processed_satir[]);

#include "compression.h"
#include "hash.h"
#include "preprocess.h"
#include "search.h"
#include "storage.h"
#include "utils.h"

#endif