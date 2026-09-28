#include <stdlib.h>
#include <string.h>
#include "strset.h"

// djb2 hash
static unsigned long hash_str(const char* s) {
    unsigned long h = 5381;
    int c;
    while ((c = (unsigned char)*s++)) {
        h = ((h << 5) + h) + c;
    }
    return h;
}

StrSet* strset_create(unsigned long bucket_count) {
    if (bucket_count == 0) bucket_count = 1024;
    StrSet* set = (StrSet*)malloc(sizeof(StrSet));
    if (set == NULL) return NULL;

    set->buckets = (StrSetNode**)calloc(bucket_count, sizeof(StrSetNode*));
    if (set->buckets == NULL) {
        free(set);
        return NULL;
    }
    set->bucket_count = bucket_count;
    set->size = 0;
    return set;
}

int strset_add(StrSet* set, const char* key) {
    if (set == NULL || key == NULL) return 0;

    unsigned long idx = hash_str(key) % set->bucket_count;
    for (StrSetNode* n = set->buckets[idx]; n != NULL; n = n->next) {
        if (strcmp(n->key, key) == 0) {
            return 0; // zaten var
        }
    }

    StrSetNode* node = (StrSetNode*)malloc(sizeof(StrSetNode));
    if (node == NULL) return 0;
    node->key = strdup(key);
    if (node->key == NULL) {
        free(node);
        return 0;
    }
    node->next = set->buckets[idx];
    set->buckets[idx] = node;
    set->size++;
    return 1; // yeni eklendi
}

void strset_free(StrSet* set) {
    if (set == NULL) return;
    for (unsigned long i = 0; i < set->bucket_count; i++) {
        StrSetNode* n = set->buckets[i];
        while (n != NULL) {
            StrSetNode* next = n->next;
            free(n->key);
            free(n);
            n = next;
        }
    }
    free(set->buckets);
    free(set);
}
