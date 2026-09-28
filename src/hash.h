#ifndef HASH_H
#define HASH_H

#include <stdio.h>

void sha256_hash(const char *input, unsigned char *output_digest, unsigned int *digest_len);

#endif