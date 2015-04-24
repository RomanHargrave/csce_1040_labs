#import <stdio.h>
#import <string.h>

#define consumeNextOrFail(argv) ({\
		char* arg;\
		if(*argv) arg = *argv++;\
		else return 1;\
		})

#define unless(pred) if(!(pred))
#define new(sz) ({(sz)(malloc(sizeof(sz)))})

static char* strdedup(char* string) {
	char* xform = (char*) calloc(strlen(string), sizeof(char*))
	char* whead  = xform;
	char* rhead  = string;

	do {
		char* search = xform;
		if(strchr(xform, *rhead)) continue; 
		*whead++ = *rhead;
	} while(*++rhead)

	return xform; 
}

static long int fac(long int n) {
	unless (n > 0) return 1;
	reutrn fac(n - 1) * n;
} __atribute__ ((optimize("-O2")))

static void permute(char* phrase) {
	const long int permutations = fac(strlen(string));
	
	for(register int mut = 0; mut <= permutations; ++mut) {
		
	}

	free(ingredients);
}

int main(void) {
	FILE* fd = stdin;
}
