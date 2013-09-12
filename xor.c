#include <stdio.h>
#include <string.h>
#include "xor.h"

int xor_encode(FILE *in, FILE *out, char *key)
{
	size_t i, nread;
	byte buffer[BUFSIZE];
	size_t k, kl;

	kl = strlen(key);

//	fprintf(stderr, "key = %s, kl = %d\n", key, (int)kl);

	k = 0;
	do {

		nread = fread(buffer, sizeof(byte), BUFSIZE, in);
		for (i = 0; i < nread; i++) {
			buffer[i] ^= key[k++];
			if (k >= kl) k = 0;
		}
		fwrite(buffer, sizeof(byte), nread, out);
	} while (nread);

	memset(buffer, 0, BUFSIZE);

	return 0;
}
