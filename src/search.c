#include "sdk.h"
#include <stdint.h>
#include <string.h>
#include <stdlib.h>


long rawData_search(char* kelime,FILE* aranacak_dosya){
    char* record;
    fseek(aranacak_dosya, 0, SEEK_SET);


    long pozisyon = ftell(aranacak_dosya);
    while((record = satiroku(aranacak_dosya)) != NULL){
        if(strcmp(kelime,record) == 0){
            free(record);
            return pozisyon;
        }
        pozisyon = ftell(aranacak_dosya);
        free(record);
    }

    return -1;
}


long hashed_data_search(char* kelime,FILE* aranacak_dosya){
    if(aranacak_dosya == NULL || kelime == NULL) return -1;

    unsigned char digest[EVP_MAX_MD_SIZE];
    unsigned int digest_len = 0;

    sha256_hash(kelime, digest, &digest_len);

    char hex_digest[65];
    for(unsigned int i = 0; i < digest_len; i++){
        sprintf(&hex_digest[i*2], "%02x", digest[i]);
        
    }
    hex_digest[64] = '\0';

    fseek(aranacak_dosya, 0, SEEK_SET);
    char* hashed_satir = NULL;
    
    long pozisyon = ftell(aranacak_dosya);
    while((hashed_satir = satiroku(aranacak_dosya)) != NULL){
        

        if(strcmp(hashed_satir, hex_digest) == 0){
            free(hashed_satir);
            return pozisyon;
        }
        free(hashed_satir);
        pozisyon = ftell(aranacak_dosya);
    }

    return -1;
}

long search_compressed_file(char* kelime, FILE* aranacak_dosya){
    if(aranacak_dosya == NULL || kelime == NULL) return -1;

    fseek(aranacak_dosya, 0, SEEK_SET);
    char* okunan_sikismis_veri;

    long index = ftell(aranacak_dosya);
    while((okunan_sikismis_veri = dosyadan_oku_ve_ac(aranacak_dosya)) != NULL){
        if(strcmp(kelime, okunan_sikismis_veri) == 0){
            free(okunan_sikismis_veri);
            return index;
        }
        free(okunan_sikismis_veri);
        index = ftell(aranacak_dosya);

    }
    

    return -1;
}

