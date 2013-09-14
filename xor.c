#include <stdio.h>
#include <string.h>
#include "xor.h"

/* XOR input file with the key data and write to the output */
int xor_encode(FILE *in, FILE *out, byte *key, size_t keysize)
{
	byte buffer[BUFSIZE];
	size_t i, nread, k;

	k = 0;
	do {

		nread = fread(buffer, sizeof(byte), BUFSIZE, in);
		for (i = 0; i < nread; i++) {
			buffer[i] ^= key[k++];
			/* Cycle through the key data */
			if (k >= keysize) k = 0;
		}
		fwrite(buffer, sizeof(byte), nread, out);
	} while (nread);

	memset(buffer, 0, BUFSIZE);

	return 0;
}

/* XOR input file with the pad and write to the output.
   The output size is the minimal of the input and the pad */
int xor_encode_pad(FILE *in, FILE *out, FILE *pad)
{
	size_t ni, np, nread, i;

	byte inbuf[BUFSIZE], padbuf[BUFSIZE];

	do {
		ni = fread(inbuf, sizeof(byte), BUFSIZE, in);
		np = fread(padbuf, sizeof(byte), BUFSIZE, pad);
		nread = (ni < np) ? ni : np;
		for (i = 0; i < nread; i++) {
			inbuf[i] ^= padbuf[i];
		}
		fwrite(inbuf, sizeof(byte), nread, out);
	} while (nread);

	memset(inbuf, 0, BUFSIZE);
	memset(padbuf, 0, BUFSIZE);

	return 0;
}
