#include "compression.h"
#include "preprocess.h"

void dosya_yaz(FILE* file, char satir[]){
    if((satir == NULL) || strlen(satir) < 7 || file == NULL){
        return;
    }
        
    if(tekrar_kontrol(file, satir)){   
        fseek(file, 0, SEEK_END);
        fprintf(file,"%s\n",satir);
        fflush(file);
    } 
}

void dosyaya_sikistirarak_yaz(FILE* dosya, const char* ham_veri) {
    if(dosya == NULL || ham_veri == NULL) return;

    unsigned long orijinal_boyut = strlen(ham_veri) + 1; // \0 dahil
    unsigned char* sikismis_veri = NULL;
    unsigned long sikismis_boyut = 0;

    if (sikistir((const unsigned char*)ham_veri, orijinal_boyut, &sikismis_veri, &sikismis_boyut) == Z_OK) {
        
        fseek(dosya, 0, SEEK_END);
        // Meta verileri yazıyoruz
        fwrite(&orijinal_boyut, sizeof(unsigned long), 1, dosya);
        fwrite(&sikismis_boyut, sizeof(unsigned long), 1, dosya);
        // Sıkışmış datayı yazıyoruz
        fwrite(sikismis_veri, 1, sikismis_boyut, dosya);

        fflush(dosya);
        
        free(sikismis_veri); // malloc ile ayrılan yeri temizle
    }
}
