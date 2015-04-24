#include <stdio.h>
#include <string.h>
#define alpha(chr) (chr - 'a')
#define unless(pred) if(!(pred))
static void 
encrypt (cipher, input, output)
	char cipher[26];
	char* input;
	char* output;
{
	do {
		*output++ = ({ char r = *input;
			if(isalpha(r)) r = cipher[alpha(r)];
			r;
		});
	} while (*++input);
}
int 
main (void) 
{
	FILE* fd = stdin;
	char cipher[26];
	{
		int posmap[26];
		{
			int pos = 0;
			do {
				char scan;
				fscanf(fd, "%c", &scan);
				unless(isalpha(scan)) continue;
				posmap[pos++] = alpha(scan);
			} while (pos < 26);
		}
		{
			int pos = 0;
			do {
				char scan;
				fscanf(fd, "%c", &scan);
				unless(isalpha(scan)) continue;
				cipher[posmap[pos++]] = scan;
			} while (pos < 26);
		}
	}
	{
		char line[1024];
		while(fgets(line, 1023, fd)) {
			const int length = strlen(line);
			if(line[length - 1] == '\n') line[length - 1] = '\0';
			unless(length > 2) continue;
			char* line_chopped = line + 2;
			char encrypted[strlen(line_chopped)];
			encrypt(cipher, line_chopped, encrypted);
			printf("%s\n%s\n", line_chopped, encrypted);
		}
	}
	return(0);
}
