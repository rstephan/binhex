/**
@brief Convert from binary to hex-headers

You can use the output to pipe it into a file. The copyright is written to 
stderr so it won't be in the output.


-------------------------------------------------------------------------------
usage:
./binhex -f in.htm -p FILE1 -t "FS_NOCACHE | MIME_TEXT_PLAIN"

Output:
binhex v1.0, Juni 2009, by Stephan Ruloff
converting in.htm ...
#define FILE1_NAME "in.htm"
#define FILE1_SIZE 7
#define FILE1_TYPE FS_NOCACHE | MIME_TEXT_PLAIN
#define FILE1_DATA in_htm
const char in_htm[] = {
0x5B, 0x40, 0x30, 0x35, 0x5D, 0x0D, 0x0A
};
-------------------------------------------------------------------------------

@author Stephan Ruloff
@version 2.1
@date Juni 2009 - 2016
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include <libgen.h>

#define hex_delimiter  ", "
#define hex_linestart  ""
#define hex_lineend    "\n"
#define hex_foot       "\n};\n"

void usage(void)
{
	printf("binhex -f <bin-file> -p <prefix> [-t <type>] [-s <style>] [-q]\n");
	printf("  -f        bin-file\n");
	printf("  -p prefix Prefix for the #define e.g. FILE1, FILE2a, ...\n"); 
	printf("  -t type   Extra file-attribute, MIME-type, ...\n");
	printf("            e.g. FS_NOCACHE | MIME_TEXT_PLAIN\n");
	printf("  -s style  None or \"avr\"\n");
	printf("  -q        Quiet, no output.\n");
	putchar('\n');
}

size_t filesize(char * filename)
{
	FILE *f;
	size_t size;

	f = fopen(filename, "rb");
	if (f == NULL) {
		return -1;
	}
	fseek(f, 0, SEEK_END);
	size = ftell(f);
	fclose(f);

	return (size_t)size;
}

void make_underscore_filename(char *us_fname)
{
	size_t i;

	for (i = 0; i < strlen(us_fname); i++) {
		if (us_fname[i] == '.' || us_fname[i] == '/') {
			us_fname[i] = '_';
		}
	}
}

int main(int argc, char **argv) 
{
	FILE *f;
	int c;
	char *filename = NULL;
	char *prefix = NULL;
	char *type = NULL;
	char *style = "";
	size_t size = 0;
	char *underscore_filename = 0;
	int cnt = 0;
	char quiet = 0;

	while ((c = getopt(argc, argv, "f:p:t:s:q")) != -1) {
		switch (c) {
		case 'f':
			filename = optarg;
			break;
		case 'p':
			prefix = optarg;
			break;
		case 't':
			type = optarg;
			break;
		case 's':
			style = optarg;
			break;
		case 'q':
			quiet = 1;
			break;
		}
	}

	if (!quiet) {
		fprintf(stderr, "binhex v2.1, Juni 2009 - 2016, by Stephan Ruloff\n");
	}

	if (filename == NULL || prefix == NULL) {
		usage();
		exit(1);
	}

	f = fopen(filename, "rb");
	if (f == NULL) {
		perror("binhex");
		exit(2);
	}
	size = filesize(filename);
	
	if (!quiet) {
		fprintf(stderr, "converting %s ...\n", filename);
	}
	underscore_filename = strdup(basename(filename));
	make_underscore_filename(underscore_filename);
	printf("#define %s_NAME \"%s\"\n", prefix, basename(filename));
	printf("#define %s_SIZE %li\n", prefix, size);
	if (type) {
		printf("#define %s_TYPE %s\n", prefix, type);
	}
	printf("#define %s_DATA %s\n", prefix, underscore_filename);

	if (strcmp(style, "avr") == 0) {
		printf("const uint8_t PROGMEM %s[] = {\n", underscore_filename);
	} else {
		printf("const unsigned char %s[] = {\n", underscore_filename);
	}
	while ((c = fgetc(f)) != EOF) {
		if (cnt == 0) {
			printf("%s", hex_linestart);
		}
		if (cnt > 0) {
			printf("%s", hex_delimiter);
		}
		if ((((cnt) % 16) == 0) && (cnt > 0)) {
			printf("%s", hex_lineend);
			printf("%s", hex_linestart);
		}
		printf("0x%02X", (unsigned char)c);
		cnt++;
	}
	printf("%s", hex_foot);
	if (underscore_filename) {
		free(underscore_filename);
	}

	return 0;
}
