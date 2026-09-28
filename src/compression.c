#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>      
#include "compression.h"

// Veriyi sıkıştıran fonksiyon
// unsigned char** dest: Sıkıştırılmış verinin yazılacağı dinamik dizi pointer'ı
// unsigned long* destLen: Sıkıştırılmış verinin nihai boyutu (fonksiyon içinde doldurulur)
int sikistir(const unsigned char *src, unsigned long srcLen, unsigned char **dest, unsigned long *destLen) {
    // zlib'in ihtiyaç duyduğu maksimum güvenli boyutu hesaplıyoruz
    unsigned long max_buffer_size = compressBound(srcLen);
    *dest = (unsigned char *)malloc(max_buffer_size);
    
    if (*dest == NULL) {
        printf("Bellek tahsis edilemedi!\n");
        return Z_MEM_ERROR;
    }

    *destLen = max_buffer_size;

    // compress fonksiyonu sıkıştırmayı yapar ve *destLen değerini gerçek sıkışmış boyutla günceller
    int sonuc = compress(*dest, destLen, src, srcLen);
    
    if (sonuc != Z_OK) {
        printf("Sıkıştırma hatası! Kod: %d\n", sonuc);
        free(*dest);
    }
    
    return sonuc; // Z_OK dönerse işlem başarılıdır
}

// Sıkıştırılmış veriyi geri açan fonksiyon
int veri_ac(const unsigned char *src, unsigned long srcLen, unsigned char *dest, unsigned long *destLen) {
    // destLen: açılacak verinin beklenen orijinal boyutu olarak gönderilmeli
    int sonuc = uncompress(dest, destLen, src, srcLen);
    return sonuc; // Z_OK ise başarılı
}


char* dosyadan_oku_ve_ac(FILE* dosya) {
    unsigned long orijinal_boyut = 0;
    unsigned long sikismis_boyut = 0;

    // Önce meta verileri sırayla dosyadan çekiyoruz
    if (fread(&orijinal_boyut, sizeof(unsigned long), 1, dosya) != 1) return NULL;
    if (fread(&sikismis_boyut, sizeof(unsigned long), 1, dosya) != 1) return NULL;

    // Diskten sıkışmış veriyi okumak için yer açıyoruz
    unsigned char* okunan_sikismis = (unsigned char*)malloc(sikismis_boyut);
    fread(okunan_sikismis, 1, sikismis_boyut, dosya);

    // Orijinal verinin geri döneceği char dizisini hazırlıyoruz
    char* acilmis_veri = (char*)malloc(orijinal_boyut);

    // Açma işlemi
    unsigned long acilan_boyut = orijinal_boyut;
    int sonuc = veri_ac(okunan_sikismis, sikismis_boyut, (unsigned char*)acilmis_veri, &acilan_boyut);

    free(okunan_sikismis);

    if (sonuc == Z_OK) {
        return acilmis_veri; // Başarılıysa metni dön (Çağıran yerde free() edilmeli!)
    } else {
        free(acilmis_veri);
        return NULL;
    }
}