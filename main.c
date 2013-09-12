#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "xor.h"

void usage(char *str)
{
	fprintf(stderr, "Usage: %s -i <input> -o <output> -k <key>\n", str);
}

int main(int argc, char **argv)
{
	FILE *in = NULL, *out = NULL;
	int c;

	char *key = NULL;

        opterr = 0;

	if (argc < 4) {
		usage(argv[0]);
		return 1;
	}

        while ((c = getopt(argc, argv, "i:o:k:")) != -1)
                switch (c) {
                        case 'i':
                                fprintf(stderr, "input = %s\n", optarg);
				in = fopen(optarg, "rb");
                                break;
                        case 'o':
				fprintf(stderr, "output = %s\n", optarg);
				out = fopen(optarg, "wb");
                                break;
			case 'k':
				fprintf(stderr, "key = %s\n", optarg);
				key = malloc(sizeof(char)*strlen(optarg));
				strcpy(key, optarg);
				break;
                        case '?':
                        default:
				usage(argv[0]);
				return 1;
                }

	if (in == NULL || out == NULL || key == NULL) {
		if (in != NULL)
			fclose(in);
		if (out != NULL) {
			fclose(out);
			remove(argv[2]);
		}
		fprintf(stderr, "I/O error!\n");
		return 1;
	}

	xor_encode(in, out, key);
	fclose(in);
	fclose(out);

	return 0;
}

