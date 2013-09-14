#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "xor.h"

void usage(char *str)
{
	fprintf(stderr, "Usage: %s <-i input> <-o output> <-s key_string|-p padfile|-k keyfile>\n", str);
}

size_t file_size(FILE *f)
{
	size_t result = 0;
	fseek(f, 0, SEEK_END);
	result = ftell(f);
	fseek(f, 0, SEEK_SET);
	return result;
}

int main(int argc, char **argv)
{
	FILE *in = NULL, *out = NULL, *pad = NULL, *key = NULL;
	byte *key_data = NULL;
	size_t key_size = 0;
	int c;

	unsigned int key_given = 0;

        opterr = 0;

        while ((c = getopt(argc, argv, "i:o:s:p:k:")) != -1)
                switch (c) {
			/* Input */
                        case 'i':
                                fprintf(stderr, "input = %s\n", optarg);
				in = fopen(optarg, "rb");
				if (!in) {
					fprintf(stderr, "Error opening input file\n");
					return 1;
				}
                                break;
			/* Output */
                        case 'o':
				fprintf(stderr, "output = %s\n", optarg);
				out = fopen(optarg, "wb");
				if (!out) {
					fprintf(stderr, "Error opening output file\n");
					return 1;
				}
                                break;
			/* Key */
			case 's':
				if (key_given) {
					usage(argv[0]);
					return 1;
				}
				fprintf(stderr, "key string = %s\n", optarg);
				key_size = strlen(optarg);
				if (key_size > 0) {
					key_data = (byte *) malloc(sizeof(char) * key_size);
					strcpy((char *) key_data, optarg);
				} else {
					fprintf(stderr, "Key string error!\n");
					return 1;
				}
				key_given = 1;
				break;
			/* Keyfile */
			case 'k':
				if (key_given) {
					usage(argv[0]);
					return 1;
				}
				fprintf(stderr, "key file = %s\n", optarg);
				key = fopen(optarg, "rb");
				if (!key) {
					fprintf(stderr, "Error reading key file\n");
					fclose(key);
					return 1;
				}
				key_size = file_size(key);
				fprintf(stderr, "key size = %lu\n", key_size);
				key_data = (byte *) malloc(sizeof(byte) * key_size);
				if (!key_data) {
					fprintf(stderr, "Error allocating memory for the key file!\n");
					return 1;
				}
				if (fread(key_data, sizeof(byte), key_size, key) != key_size) {
					fprintf(stderr, "Error reading key (size mismatch)!\n");
					return 1;
				}
				fclose(key);
				key_given = 1;
				break;
			/* One-time pad */
			case 'p':
				if (key_given) {
					usage(argv[0]);
					return 1;
				}
				fprintf(stderr, "pad = %s\n", optarg);
				pad = fopen(optarg, "rb");
				if (pad == NULL) {
					fprintf(stderr, "Error reading pad file\n");
					return 1;
				}
				key_given = 1;
				break;
			/* Everything else */
                        case '?':
                        default:
				usage(argv[0]);
				return 1;
                }

	if (!key_given) {
		fprintf(stderr, "Please specify input, output, and one of three options: -s, -p, -k\n");
		usage(argv[0]);
		return 0;
	}

	/* Do the main stuff */
	if (key_data || key) {
		xor_encode(in, out, key_data, key_size);
		free(key_data);
	} else if (pad) {
		xor_encode_pad(in, out, pad);
		fclose(pad);
	}

	fclose(in);
	fclose(out);

	fprintf(stderr, "Done.\n");
	return 0;
}
