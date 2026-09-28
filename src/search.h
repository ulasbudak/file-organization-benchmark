#ifndef SEARCH_H
#define SEARCH_H

#include <stdio.h>

long rawData_search(char* kelime,FILE* aranacak_dosya);
long hashed_data_search(char* kelime,FILE* aranacak_dosya);
long search_compressed_file(char* kelime, FILE* aranacak_dosya);

extern int olcum_sayisi; 
extern double toplam_ram;
extern double anlik_ram;

#endif