#ifndef STRSET_H
#define STRSET_H

// Basit, zincirleme (separate chaining) tabanli string hash-set.
// Tekrar kontrolunu O(n^2) dosya taramasi yerine O(n) hafizada yapar.

typedef struct StrSetNode {
    char* key;
    struct StrSetNode* next;
} StrSetNode;

typedef struct {
    StrSetNode** buckets;
    unsigned long bucket_count;
    unsigned long size;
} StrSet;

StrSet* strset_create(unsigned long bucket_count);

// Eleman zaten varsa 0, yeni eklendiyse 1 doner.
int strset_add(StrSet* set, const char* key);

void strset_free(StrSet* set);

#endif
