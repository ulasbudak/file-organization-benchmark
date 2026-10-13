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
