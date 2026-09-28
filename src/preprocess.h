#ifndef PREPROCESS_H
#define PREPROCESS_H

#include <stdio.h>

void temizle_newline(char* s);
int tekrar_kontrol(FILE* file, char processed_satir[]);
char* satiroku(FILE* file);
char* process(char satir[]);


#endif