#include <time.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#ifdef _WIN32
#include <direct.h>
#endif
#include "utils.h"

void path_join(char* out, const char* base, const char* rel) {
    if (base == NULL || base[0] == '\0' || strcmp(base, ".") == 0) {
        snprintf(out, 1024, "%s", rel);
    } else {
        snprintf(out, 1024, "%s/%s", base, rel);
    }
}

void ensure_dir(const char* path) {
#ifdef _WIN32
    _mkdir(path);
#else
    mkdir(path, 0755);
#endif
}

static clock_t start_time;

void start_timer(){
    start_time = clock();
}

double stop_timer(){
    clock_t end_time = clock();

    return (double)(end_time-start_time) / CLOCKS_PER_SEC * 1000.0;
}



int copy_file(const char *source_path, const char *dest_path) {
    FILE *src = fopen(source_path, "rb");
    if (src == NULL) {
        printf("Hata: Kaynak dosya acilamadi! (%s)\n", source_path);
        return 0;
    }

    FILE *dst = fopen(dest_path, "wb");
    if (dst == NULL) {
        printf("Hata: Hedef dosya olusturulamadi! (%s)\n", dest_path);
        fclose(src); 
        return 0;
    }


    char buffer[BUFSIZ];
    size_t okunacak_byte;
    while ((okunacak_byte = fread(buffer, 1, sizeof(buffer), src)) > 0) {
    
        fwrite(buffer, 1, okunacak_byte, dst);
    }

 
    fclose(src);
    fclose(dst);

    return 1;
}








long get_file_size(const char* filename){
    FILE* file = fopen(filename, "rb");
    if(file == NULL){
        printf("Dosya boyutu hesaplamak icin dosya acilamadi.");
        return -1;
    }
    long file_size;

    fseek(file, 0, SEEK_END);
    file_size = ftell(file);
    fclose(file);

    return file_size;
}


#ifdef _WIN32
#include <windows.h>

static ULONGLONG FileTimeToQuadWord(PFILETIME pft){
    return ((((ULONGLONG)pft->dwHighDateTime)<< 32) | pft->dwLowDateTime);
}

double get_cpu_usage_windows(){
    static int first_run = 1;
    static ULONGLONG last_cpu_time = 0;
    static ULONGLONG last_wall_time = 0;

    FILETIME creation_time,exit_time, kernel_time, user_time;
    SYSTEMTIME sys_time;
    FILETIME wall_time;

    GetProcessTimes(GetCurrentProcess(), &creation_time, &exit_time, &kernel_time, &user_time);
    GetSystemTime(&sys_time);
    SystemTimeToFileTime(&sys_time, &wall_time);

    ULONGLONG cpu_time = FileTimeToQuadWord(&kernel_time) + FileTimeToQuadWord(&user_time);
    ULONGLONG wall_t = FileTimeToQuadWord(&wall_time);

    if(first_run){
        last_cpu_time = cpu_time;
        last_wall_time = wall_t;
        first_run = 0;
        return 0.0;
    }

    ULONGLONG cpu_diff = cpu_time - last_cpu_time;
    ULONGLONG wall_diff = wall_t - last_wall_time;

    last_cpu_time = cpu_time;
    last_wall_time = wall_t;

    if(wall_diff == 0) return 0.0;
    return ((double)cpu_diff / wall_diff) * 100.0;
}


#else
#include <sys/resource.h>
#include <sys/time.h>

double get_cpu_usage_posix(){
    static int first_run = 1;
    static struct timeval last_cpu_time;
    static struct timeval last_wall_time;

    struct rusage usage;
    struct timeval wall_time;

    getrusage(RUSAGE_SELF, &usage);
    gettimeofday(&wall_time, NULL);

    struct timeval cpu_time;
    timeradd(&usage.ru_utime, &usage.ru_stime, &cpu_time);

    if(first_run){
        last_cpu_time = cpu_time;
        last_wall_time = wall_time;
        first_run = 0;
        return 0.0;
    }

    double cpu_diff = (cpu_time.tv_sec - last_cpu_time.tv_sec) + (cpu_time.tv_usec - last_cpu_time.tv_usec) /1000000.0;
   
    double wall_diff = (wall_time.tv_sec - last_wall_time.tv_sec) + (wall_time.tv_usec - last_wall_time.tv_usec) / 1000000.0;

    last_cpu_time = cpu_time;
    last_wall_time = wall_time;

    if(wall_diff == 0.0) return 0.0;
    return (cpu_diff / wall_diff) * 100.0;
}

#endif

double get_cpu_usage(){
#ifdef _WIN32
    return get_cpu_usage_windows();
#else
    return get_cpu_usage_posix();
#endif
}



#ifdef _WIN32
#include <windows.h>
#include <psapi.h>

double get_ram_usage() {
    PROCESS_MEMORY_COUNTERS pmc;
    if (K32GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc))) {
        // WorkingSetSize byte cinsindendir, MB'a çeviriyoruz (1024 * 1024)
        return (double)pmc.WorkingSetSize / (1024.0 * 1024.0);
    }
    return -1.0;
}

#elif __APPLE__
#include <mach/mach.h>

double get_ram_usage() {
    struct mach_task_basic_info info;
    mach_msg_type_number_t count = MACH_TASK_BASIC_INFO_COUNT;
    
    if (task_info(mach_task_self(), MACH_TASK_BASIC_INFO, (task_info_t)&info, &count) == KERN_SUCCESS) {
        // resident_size byte cinsindendir, MB'a çeviriyoruz
        return (double)info.resident_size / (1024.0 * 1024.0);
    }
    return -1.0;
}

#else // LINUX
#include <stdio.h>
#include <unistd.h>

double get_ram_usage() {
    FILE* fp = fopen("/proc/self/statm", "r");
    if (fp == NULL) return -1.0;

    long pages = 0;
    // statm dosyasındaki ikinci değer RSS (Resident Set Size) yani fiziksel RAM'dir
    if (fscanf(fp, "%*s %ld", &pages) != 1) {
        fclose(fp);
        return -1.0;
    }
    fclose(fp);

    // Sistemdeki bir sayfanın (page) kaç byte olduğunu buluyoruz (Genelde 4096 byte)
    long page_size = sysconf(_SC_PAGESIZE);
    
    // Toplam byte değerini Megabyte'a çeviriyoruz
    return (double)(pages * page_size) / (1024.0 * 1024.0);
}
#endif



