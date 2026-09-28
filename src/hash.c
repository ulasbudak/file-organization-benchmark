#include <stdio.h>
#include <string.h>
#include <openssl/evp.h>

void sha256_hash(const char *input, unsigned char *output_digest, unsigned int *digest_len) {
    // EVP API için context (bağlam) oluşturuluyor
    EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
    
    if (mdctx == NULL) {
        printf("Context oluşturulamadı.\n");
        return;
    }

    // SHA-256 algoritması ile hash işlemi başlatılıyor
    if (EVP_DigestInit_ex(mdctx, EVP_sha256(), NULL) != 1) {
        printf("Digest başlatılamadı.\n");
        EVP_MD_CTX_free(mdctx);
        return;
    }

    // Veri algoritmaya besleniyor (Metin uzunsa bu fonksiyon ardı ardına çağrılabilir)
    if (EVP_DigestUpdate(mdctx, input, strlen(input)) != 1) {
        printf("Digest güncellenemedi.\n");
        EVP_MD_CTX_free(mdctx);
        return;
    }

    // Hash işlemi tamamlanıyor ve sonuç output_digest dizisine yazılıyor
    if (EVP_DigestFinal_ex(mdctx, output_digest, digest_len) != 1) {
        printf("Digest tamamlanamadı.\n");
        EVP_MD_CTX_free(mdctx);
        return;
    }

    // Belleği temizle
    EVP_MD_CTX_free(mdctx);
}

/*
int main() {
    
    FILE* processed_file = fopen("/Users/ulasbudak/Desktop/odev/Project/Processed/Processed.txt","r");
    FILE* hashed_file = fopen("/Users/ulasbudak/Desktop/odev/Project/Storage/Hashed/Hashed.txt","w+"); 

    if(hashed_file == NULL){
        printf("Hashed file couldn't be opened.");
        return 1;
    }

    if(processed_file == NULL){
        printf("Processed file couldn't be opened.");
        return 1;
    }
    
    // SHA-256 çıktısı her zaman 32 byte (256 bit) uzunluğundadır
    unsigned char digest[EVP_MAX_MD_SIZE];
    unsigned int digest_len = 0;

    char* processed_satir;
    unsigned char hashed_satir[32];

    while((processed_satir = satiroku(processed_file)) != NULL){
        sha256_hash(processed_satir, digest, &digest_len);
        for(unsigned int i = 0; i < digest_len; i++){
            hashed_satir[i] = digest[i];
        }
        fprintf(hashed_file,"%02x\n", hashed_satir);
    }

    

    return 0;
}

*/