#ifndef STORAGE_H
#define STORAGE_H

#include <stdio.h>

void dosya_yaz(FILE* file, char satir[]);
void dosyaya_sikistirarak_yaz(FILE* dosya, const char* ham_veri);

#endif