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

	/* Not enough command line parameters */
	if (argc < 4) {
		usage(argv[0]);
		return 1;
	}

        while ((c = getopt(argc, argv, "i:o:k:")) != -1)
                switch (c) {
			/* Input */
                        case 'i':
                                fprintf(stderr, "input = %s\n", optarg);
				in = fopen(optarg, "rb");
                                break;
			/* Output */
                        case 'o':
				fprintf(stderr, "output = %s\n", optarg);
				out = fopen(optarg, "wb");
                                break;
			/* Key */
			case 'k':
				fprintf(stderr, "key = %s\n", optarg);
				key = malloc(sizeof(char)*strlen(optarg));
				strcpy(key, optarg);
				break;
			/* Everything else */
                        case '?':
                        default:
				usage(argv[0]);
				return 1;
                }

	/* Make an extra check before I/O operations */
	if (in == NULL || out == NULL || key == NULL) {
		if (in != NULL)
			fclose(in);
		if (out != NULL)
			fclose(out);
		fprintf(stderr, "I/O error!\n");
		return 1;
	}

	xor_encode(in, out, key);

	fclose(in);
	fclose(out);
	return 0;
}
