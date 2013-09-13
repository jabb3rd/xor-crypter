#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "xor.h"

void usage(char *str)
{
	fprintf(stderr, "Usage: %s -i <input> -o <output> (-k <key> | -p <padfile>)\n", str);
}

int main(int argc, char **argv)
{
	FILE *in = NULL, *out = NULL, *pad = NULL;
	char *key = NULL;
	int c;
        opterr = 0;

	/* Not enough command line parameters */
	if (argc < 4) {
		usage(argv[0]);
		return 1;
	}

        while ((c = getopt(argc, argv, "i:o:k:p:")) != -1)
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
			/* One-time pad */
			case 'p':
				fprintf(stderr, "pad = %s\n", optarg);
				pad = fopen(optarg, "rb");
				break;
			/* Everything else */
                        case '?':
                        default:
				usage(argv[0]);
				return 1;
                }

	/* Make an extra check before I/O operations */
	if (in == NULL || out == NULL) {
		if (in != NULL)
			fclose(in);
		if (out != NULL)
			fclose(out);
		fprintf(stderr, "I/O error!\n");
		return 1;
	}

	if (key == NULL && pad == NULL) {
		fprintf(stderr, "Key error!\n");
		usage(argv[0]);
		if (pad != NULL)
		    fclose(pad);
		return 1;
	}

	if (key != NULL && pad != NULL) {
		fprintf(stderr, "Both pad file and key are given, we need only one of them!\n");
		usage(argv[0]);
		fclose(pad);
		return 1;
	}

	/* Do the main stuff */
	if (key != NULL)
		xor_encode(in, out, key);
	else if (pad != NULL) {
		xor_encode_pad(in, out, pad);
		fclose(pad);
	}
	fclose(in);
	fclose(out);

	fprintf(stderr, "Done.\n");
	return 0;
}
