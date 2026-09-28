#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>

long get_file_size(const char* filename);

double get_cpu_usage();

double get_ram_usage(); // RAM kullanımını MB (Megabyte) cinsinden döndürür

void start_timer();
double stop_timer();

int copy_file(const char *source_path, const char *dest_path);

// base ve rel'i birlestirip out'a yazar (out en az 1024 byte olmali).
void path_join(char* out, const char* base, const char* rel);

// Dizin yoksa olusturur (parent'lar zaten var kabul edilir).
void ensure_dir(const char* path);

#endif