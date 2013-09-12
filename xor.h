#ifndef __XOR_H
#define __XOR_H
#include <stdio.h>

#define BUFSIZE 16384
typedef unsigned char byte;

int xor_encode(FILE *in, FILE *out, char *key);

#endif /* __XOR_H */

