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




/*

int main() {


    printf("--- 1. AŞAMA: VERİYİ SIKIŞTIRMA VE DOSYAYA YAZMA ---\n");

    const char *dosya_adi = "/Users/ulasbudak/Desktop/compressed_data.bin";

    FILE* processed_dosya = fopen("/Users/ulasbudak/Desktop/odev/Project/Processed/Processed.txt", "r");
    FILE *yazilacak_dosya = fopen(dosya_adi, "wb");
    
    
    if (yazilacak_dosya == NULL) {
        printf("Dosya yazma modunda acilamadi!\n");
        return 1;
    }

    if (processed_dosya == NULL) {
        printf("Dosya okuma modunda acilamadi!\n");
        return 1;
    }

    static char orijinal_veri[256];

    while(fgets(orijinal_veri, sizeof(orijinal_veri), processed_dosya) != NULL){

        unsigned long orijinal_boyut = strlen(orijinal_veri) + 1; // '\0' (null karakteri) dahil ediyoruz
        unsigned char *sikismis_veri = NULL;
        unsigned long sikismis_boyut = 0;

        // Sıkıştırma fonksiyonunu çağırıyoruz
        if (sikistir((const unsigned char*)orijinal_veri, orijinal_boyut, &sikismis_veri, &sikismis_boyut) == Z_OK) {
            printf("Orijinal Boyut: %lu byte\n", orijinal_boyut);
            printf("Sıkıştırılmış Boyut: %lu byte\n", sikismis_boyut);

            // 1. Orijinal boyutu yaz (Açarken ne kadar yer ayıracağımızı bilmek için)
            fwrite(&orijinal_boyut, sizeof(unsigned long), 1, yazilacak_dosya);
            
            // 2. Sıkıştırılmış boyutu yaz (Diskten kaç byte okuyacağımızı bilmek için)
            fwrite(&sikismis_boyut, sizeof(unsigned long), 1, yazilacak_dosya);
            
            // 3. Sıkıştırılmış ham byte verisini yaz
            fwrite(sikismis_veri, 1, sikismis_boyut, yazilacak_dosya);

            printf("Veri basariyla '%s' dosyasina sikistirilarak yazildi.\n\n", dosya_adi);
            
            // Belleği temizle
            free(sikismis_veri);
        }
    }

    fclose(yazilacak_dosya);


    printf("--- 2. AŞAMA: DOSYADAN OKUMA VE SIKIŞTIRMAYI AÇMA (DECOMPRESS) ---\n");

    // "rb" -> Read Binary (İkili okuma modu)
    FILE *okunacak_dosya = fopen(dosya_adi, "rb");
    if (okunacak_dosya == NULL) {
        printf("Dosya okuma modunda acilamadi!\n");
        return 1;
    }

    char* cozulden_satir = NULL;

    while ((cozulden_satir = dosyadan_oku_ve_ac(okunacak_dosya)) != NULL) {
        printf("Elde Edilen Satır: %s\n", cozulden_satir);
        free(cozulden_satir); // Fonksiyon içindeki malloc'u burada temizliyoruz
    }
    
    fclose(okunacak_dosya);


    return 0;
}

*/