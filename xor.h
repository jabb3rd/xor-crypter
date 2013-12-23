#ifndef __XOR_H
#define __XOR_H
#include <stdio.h>

#define BUFSIZE 16384
typedef unsigned char byte;

int xor_encode(FILE *in, FILE *out, byte *key, size_t keysize);
int xor_encode_pad(FILE *in, FILE *out, FILE *pad);

#endif /* __XOR_H */
