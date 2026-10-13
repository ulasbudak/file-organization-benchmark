#include <stdio.h>
#include "sdk.h"

void temizle_newline(char* s){
    if(s){
    s[strcspn(s,"\n")] = '\0';
    s[strcspn(s,"\r")] = '\0';
    }
}

int tekrar_kontrol(FILE* file, char processed_satir[]){
    if(processed_satir == NULL || strlen(processed_satir) < 7) return 0;

    char temp[256];
    fseek(file, 0, SEEK_SET);

    while(fgets(temp,sizeof(temp), file)){
        temizle_newline(temp);

        if(!strcmp(processed_satir, temp)){
            return 0;
        }
    }
    return 1;
}


char* satiroku(FILE* file){

    char* raw_satir = (char*)malloc(256 * sizeof(char));
    if(raw_satir == NULL) return NULL;

    if(fgets(raw_satir, 256, file) != NULL){
        temizle_newline(raw_satir);
        return raw_satir;
    }
    free(raw_satir);
    return NULL;
}

char* process(char satir[]){
    if(satir == NULL) return NULL;

    int sayi=0,harf=0;
    char* islenmis_satir = (char*)calloc(256, sizeof(char));
    if(islenmis_satir == NULL) return NULL;
    
    int j = 0;
    for(int i = 0; satir[i] != '\0'; i++){
        if(satir[i] >= '0' && satir[i] <= '9'){
            islenmis_satir[j++] = satir[i];
            sayi++;
        }

        else if(satir[i] >= 'a' && satir[i] <= 'z'){
            islenmis_satir[j++] = satir[i];
            harf++;
        }
    }

    islenmis_satir[j] = '\0';
    
    if((j >= 7) && (sayi > 0) && (harf > 0))
        return islenmis_satir;

    else{
        free(islenmis_satir);
        return NULL;
    }
}
