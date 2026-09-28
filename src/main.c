#include "sdk.h"
#include "strset.h"

#define SIZE 200

double anlik_ram;
double toplam_ram = 0.0;
int olcum_sayisi = 0;

char* aranacak_sifreler[] = {"matrix","susanne","schule","rammstein","Alexander","123456!","123456a","eminem","qwertyuiop","wolfgang","medion","steffi","engelchen",
"claudia","deutschland","minecraft","meister","bushido","qwerty123","oliver","fuckyou","florian1","slipknot","stefanie","annalena","12345655","1qaz2wsx","garfield",
"siemens","christina","steffen","cocacola","qwertz12","daniela","manuel","Sonnenschein","mamapapa","swordfish","killer123","robert","123qwe","marvin","porsche",
"kerstin","88ggOx8ouG","schokolade","stargate","metallica","alexander1","martina","monika","30media","niklas","passwort123","123456789a","handball1","hannover",
"11111111","qwertz123","natascha","88888888","19weed","VQsaBLPzLa","test123","1q2w3e","onkelz","porsche911","kawasaki","alexandra","blacky","24crow","10pace",
"michaela","kinder","schnuffi","speedy","Fussball","schnuffel","playboy","wertz123","hallohallo","hamster","arschloch1","1a2b3c4d","monster","banane","hallo1234",
"asdasd","654321","Feuerwehr","eintracht","saskia","frosch","familie","12qwasyx","schlampe","scheisse","undertaker","angelika","64t3zaWonZ","Michael","asdfasdf",
"schnucki","Dortmund","sophie","59mile","ficken123","123abc","121212","aaaaaa","stuttgart","87654321","computer1","superman","skorpion","wuschel","christoph",
"Jolinachiara1","dortmund09","Christian","Dlnns73x7P","christian1","katrin","schneider","simone","andreas1","manuela","Michelle","leonie","789456123","charlie",
"johanna","satan666","ferrari","fussball1","hardcore","blabla","marina","Handball","warcraft3","Y52flzfq8V","marlboro","warcraft","pokemon1","321meins","Maximilian",
"patrick1","Johannes","vAl353nxhC","sdfsb5fgikd","Sebastian","Y57e4lukgH","heinrich","internet1","hallo12","hamburg1","Borussia","franziska","gangster","Matthias",
"Florian","wertzu","Mercedes","winter","diablo","shadow","tennis","LuckyLuke","melissa","apollo13","baerchen","nirvana","08154711","peter123","Benjamin","dominik1",
"telefon","Daniel","Sonnenblume","1qayxsw2","kruemel","hannah","ronaldo","Arschloch","222222","123321","pallmall","bushido1","lollol","felix123","mallorca","Katharina",
"Hannover96","franzi","steven","nintendo","schlange","159753","7777777","P0FJVV2D5969","Kentucky1","19111988","patrick2010","Nadine14","geli345hi67","mr874426","Thekings",
"hildegart","21071978","Qgsgyt12","nummer111","065788JMu","harry.styles.","zahlen123","Lord2016","yannick1995","03051982","hallowiegehts","engel158","1krischi!","michael00",
"baerchen!","damnit1","1964Urfeld","Cookie123","anMax1020","Igby2004","Robert28121990","golfgti1802","torwart2000","jennifer84","charlie92","andres123","Trust_no1","funkyboy",
"tommyy95","nandan13","Holly377","171287","Isabella96","Angela2000","checker4529","260579","Marianne2011","juliane89","Biene1","Paladin12","Sonne100","agnusdei","husqvarna125",
"patrick111"};



// Ham veriyi isler, filtreler ve tekrarlari O(n) hafiza-tabanli set ile eleyerek yazar.
void main_process(FILE* old, FILE* new){
    if(new == NULL){
        printf("Yeni dosya açilamadi\n");
        return;
    }

    if(old == NULL){
        printf("Eski dosya açilmadi\n");
        return;
    }

    StrSet* seen = strset_create(131072);

    char* satir;
    while((satir = satiroku(old)) != NULL){

        char* islenmis_satir = process(satir);
        free(satir);

        if(islenmis_satir == NULL) continue;

        // strset_add yeni eleman icin 1, zaten varsa 0 doner -> tekrarlar elenir.
        if(seen == NULL || strset_add(seen, islenmis_satir)){
            fprintf(new, "%s\n", islenmis_satir);
        }
        free(islenmis_satir);
    }
    fflush(new);
    strset_free(seen);

    anlik_ram = get_ram_usage();
    toplam_ram += anlik_ram;
    olcum_sayisi++;
}


void main_hash(FILE* processed, FILE* hashed){
    if(hashed == NULL){
        printf("Hashed file couldn't be opened.\n");
        return;
    }

    if(processed == NULL){
        printf("Processed file couldn't be opened.\n");
        return;
    }

    // SHA-256 çıktısı her zaman 32 byte (256 bit) uzunluğundadır
    unsigned char digest[EVP_MAX_MD_SIZE];
    unsigned int digest_len = 0;

    char* processed_satir;

    while((processed_satir = satiroku(processed)) != NULL){
        sha256_hash(processed_satir, digest, &digest_len);
        for(unsigned int i = 0; i < digest_len; i++){
            fprintf(hashed, "%02x", digest[i]);
        }

        fprintf(hashed, "\n");
        free(processed_satir);

        anlik_ram = get_ram_usage();
        toplam_ram += anlik_ram;
        olcum_sayisi++;
    }
}


void main_compress(FILE* processed, FILE* compressed){
    if (compressed == NULL) {
        printf("Dosya yazma modunda acilamadi!\n");
        return;
    }

    if (processed == NULL) {
        printf("Dosya okuma modunda acilamadi!\n");
        return;
    }

    static char orijinal_veri[256];

    while(fgets(orijinal_veri, sizeof(orijinal_veri), processed) != NULL){

        temizle_newline(orijinal_veri);
        if(strlen(orijinal_veri) == 0) continue;

        dosyaya_sikistirarak_yaz(compressed, orijinal_veri);

    }
    anlik_ram = get_ram_usage();
    toplam_ram += anlik_ram;
    olcum_sayisi++;
}


void main_decompress(FILE* compressed, FILE* compressed_save){
    if (compressed == NULL) {
        printf("Dosya okuma modunda acilamadi!\n");
        return;
    }

    char* cozulen_satir = NULL;

    while ((cozulen_satir = dosyadan_oku_ve_ac(compressed)) != NULL) {

        fwrite(cozulen_satir,strlen(cozulen_satir), 1, compressed_save);
        fprintf(compressed_save,"\n");
        free(cozulen_satir); // Fonksiyon içindeki malloc'u burada temizliyoruz

    }
    anlik_ram = get_ram_usage();
    toplam_ram += anlik_ram;
    olcum_sayisi++;
}


int main(int argc, char** argv){

    // Tum yollar bu temel dizine gore olusturulur.
    // Varsayilan: gecerli dizin (repo koku). Alternatif: ./app <base_dir>
    const char* base = (argc > 1) ? argv[1] : ".";

    // Girdi ve cikti yollarini olustur
    char raw_path[1024], processed_path[1024], raw_copy_path[1024];
    char hashed_path[1024], compressed_path[1024], decompressed_path[1024];
    char out_dir[1024], out_raw_dir[1024], out_hashed_dir[1024], out_compressed_dir[1024];

    path_join(out_dir, base, "output");
    path_join(out_raw_dir, base, "output/Raw");
    path_join(out_hashed_dir, base, "output/Hashed");
    path_join(out_compressed_dir, base, "output/Compressed");

    ensure_dir(out_dir);
    ensure_dir(out_raw_dir);
    ensure_dir(out_hashed_dir);
    ensure_dir(out_compressed_dir);

    path_join(raw_path, base, "data/RawData.txt");
    path_join(processed_path, base, "output/Processed.txt");
    path_join(raw_copy_path, base, "output/Raw/RawData.txt");
    path_join(hashed_path, base, "output/Hashed/Hashed.txt");
    path_join(compressed_path, base, "output/Compressed/Compressed_Data.bin");
    path_join(decompressed_path, base, "output/Compressed/Decompressed.txt");


    // --- 1. AŞAMA: RAW VERİYİ PROCESS ETME SÜRECİ ---
    printf("--- 1. ASAMA: Veriler isleniyor... ---\n\n");
    fflush(stdout);

    //old_file raw datayı temsil ediyor
    //new_file ise processed olacak datayı...
    FILE* old_file = fopen(raw_path, "r");
    if(old_file == NULL){
        // Gercek corpus (data/RawData.txt) yoksa depoyla gelen sentetik ornege don.
        path_join(raw_path, base, "data/sample.txt");
        old_file = fopen(raw_path, "r");
        printf("(data/RawData.txt bulunamadi; ornek veri kullaniliyor: %s)\n\n", raw_path);
    }
    FILE* new_file = fopen(processed_path, "w+");

    main_process(old_file, new_file);

    long processed_file_size = get_file_size(processed_path);

    if(copy_file(processed_path, raw_copy_path) == 0){
        return 1;
    }

    if(new_file) fclose(new_file);
    if(old_file) fclose(old_file);

    printf("Veriler islendi ve processed.txt olarak kaydedildi.\n\n\n");


    // --- 2. AŞAMA: PROCESSED VERİYİ HASH ETME SÜRECİ ---
    printf("--- 2. ASAMA: Veriler hashleniyor... ---\n");
    fflush(stdout);


    FILE* processed_file = fopen(processed_path, "r");
    FILE* hashed_file = fopen(hashed_path, "w+");

    main_hash(processed_file, hashed_file);

    long hashed_file_size = get_file_size(hashed_path);
    if(processed_file) fclose(processed_file);
    if(hashed_file) fclose(hashed_file);

    printf("Hashing süreci tamamlandi.\n\n");



    // --- 3. AŞAMA: VERİLERİN SIKIŞTIRILMASI ---
    printf("--- 3. ASAMA: VERIYI SIKISTIRMA VE DOSYAYA YAZMA ---\n");
    fflush(stdout);


    FILE* processed_dosya = fopen(processed_path, "r");
    FILE* compressed_file = fopen(compressed_path, "wb");

    main_compress(processed_dosya, compressed_file);
    anlik_ram = get_ram_usage();
    toplam_ram += anlik_ram;
    olcum_sayisi++;

    long compressed_file_size = get_file_size(compressed_path);
    if(compressed_file) fclose(compressed_file);
    if(processed_dosya) fclose(processed_dosya);

    printf("Verilerin sıkıştırma süreci tamamlandı. \n\n");




    // --- 4. AŞAMA: SIKIŞTIRMAYI AÇMA (DECOMPRESS) ---
    printf("--- 4. ASAMA: DOSYADAN OKUMA VE SIKISTIRMAYI ACMA (DECOMPRESS) ---\n\n");
    fflush(stdout);


    FILE *okunacak_dosya = fopen(compressed_path, "rb");
    FILE* yazilacak_dosya = fopen(decompressed_path, "w+");

    start_timer();

    main_decompress(okunacak_dosya, yazilacak_dosya);

    anlik_ram = get_ram_usage();
    toplam_ram += anlik_ram;
    olcum_sayisi++;

    double decompress_time = stop_timer();

    if(okunacak_dosya) fclose(okunacak_dosya);
    if(yazilacak_dosya) fclose(yazilacak_dosya);

    printf("Verilerin geri açılması (decompressing) işlemi tamamlandı.\n\n");


    // --- 5. AŞAMA: SEARCHING METODLARININ KULLANILMASI ---


    printf("--- 5. ASAMA: ARAMA ISLEMLERI ---\n\n\n");
    fflush(stdout);

    int count = sizeof(aranacak_sifreler) / sizeof(aranacak_sifreler[0]);


    FILE* p_file = fopen(processed_path, "r");
    //processed file üzerinde sequential arama sırasında kullanılan cpu yu ölçmek için
    //başlangıç referansı alıyorum
    get_cpu_usage();
    start_timer();

    printf("Processed Data'dan arama islemi basliyor...\n\n");
    for(int i = 0; i < count; i++){
        if (aranacak_sifreler[i] == NULL) {
        printf("NULL bulundu! i = %d\n\n", i);
        continue;
    }

        long raw_veri_adres = rawData_search(aranacak_sifreler[i], p_file);

        if(raw_veri_adres == -1){
            printf("%s sifresi dosyada bulunmamaktadir.\n\n", aranacak_sifreler[i]);
            continue;
        }
        printf("Islenmis veriler içerisinde '%s' kelimesinin adresi:%ld\n\n", aranacak_sifreler[i], raw_veri_adres);

    }

    anlik_ram = get_ram_usage();
    toplam_ram += anlik_ram;
    olcum_sayisi++;

    double processed_search_cpu_usage = get_cpu_usage();
    double processed_search_time = stop_timer();

    printf("\n\nHashed Data'dan arama islemi baslatiliyor...\n\n");


    FILE* h_file = fopen(hashed_path, "rb");

    if(h_file == NULL) return 1;

    get_cpu_usage();
    start_timer();


    for(int i = 0; i < count; i++){

    long hashed_veri_adres = hashed_data_search(aranacak_sifreler[i], h_file);


    if(hashed_veri_adres == -1){
        printf("%s sifresi dosyada bulunmamaktadir.\n\n", aranacak_sifreler[i]);
        continue;
    }

    printf("Hashed veriler içerisinde '%s' kelimesinin adresi:%ld\n\n",aranacak_sifreler[i], hashed_veri_adres);

    }
    anlik_ram = get_ram_usage();
    toplam_ram += anlik_ram;
    olcum_sayisi++;

    double hashed_search_cpu_usage = get_cpu_usage();
    double hashed_search_time = stop_timer();


    printf("\nCompressed Data'dan arama islemi baslatiliyor...\n\n");

    FILE* c_file = fopen(compressed_path, "rb");

    get_cpu_usage();
    start_timer();

    for(int i = 0; i < count; i++){

        long compressed_veri_adres = search_compressed_file(aranacak_sifreler[i], c_file);

        if(compressed_veri_adres == -1){
            printf("%s sifresi dosyada bulunmamaktadir.\n\n", aranacak_sifreler[i]);
            continue;
        }

        printf("Compressed veriler içerisinde '%s' kelimesinin adresi:%ld\n\n",aranacak_sifreler[i], compressed_veri_adres);

    }
    anlik_ram = get_ram_usage();
    toplam_ram += anlik_ram;
    olcum_sayisi++;

    double compressed_search_time = stop_timer();
    double compressed_search_cpu_usage = get_cpu_usage();

    if(p_file) fclose(p_file);
    if(h_file) fclose(h_file);
    if(c_file) fclose(c_file);

    printf("\n\nArama işlemleri tamamlandı. \n\n\n");
    printf("--- 6 . ASAMA : PERFORMANS OLCUMLERI ---\n\n");

    printf("Raw Data file size: %ld byte\n",processed_file_size);
    printf("Hashed Data file size: %ld byte\n", hashed_file_size);
    printf("Compressed Data file size: %ld byte\n\n", compressed_file_size);

    double compression_ratio = 0.0;
    if(processed_file_size > 0){
        compression_ratio = (double)compressed_file_size / (double)processed_file_size;
    }
    printf("Sıkıştırma oranı (compressed / raw): %.2f\n\n", compression_ratio);

    printf("Raw data search CPU kullanım ortalamasi: %.2f%%\n", processed_search_cpu_usage);
    printf("Hashed data search CPU kullanım ortalamasi: %.2f%%\n", hashed_search_cpu_usage);
    printf("Compressed data search CPU kullanım ortalamasi: %.2f%%\n\n", compressed_search_cpu_usage);

    double ortalama_ram = 0;
    if(olcum_sayisi != 0){
        ortalama_ram = toplam_ram / olcum_sayisi;
    }


    printf("Kullanilan ortalama RAM miktari: %.2f MB\n\n", ortalama_ram);


    printf("Raw Data search time: %.2f ms\n",processed_search_time);
    printf("Hashed search time: %.2f ms\n",hashed_search_time);
    printf("Compressed Data search time: %.2f ms\n",compressed_search_time);
    printf("Decompression time: %.2f ms\n", decompress_time);

    return 0;
}
