#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int strsum(char* str) {
	int sum = 0;
	while(*str != 0) sum += *str++;
	return sum;
}

static int strcc(char* str, const char needle) {
	int count = 0;
	while(*str != 0) count += (*str++ == needle) ? 1 : 0;
	return count;
}

int main(void) {

	char** strings = (char**) malloc(sizeof(char*) * 20);

	{
		char** current = strings;
		while((current - strings) < 20) {
			int linesize = 0;
			fscanf(stdin, "%d ", &linesize); 
			++linesize /* cut -d ' ' -f2 < twentywords | xargs wc -c would beg to differ */;
			*current = (char*) calloc(sizeof(char), linesize);
			fgets(*current++, linesize, stdin);
			fseek(stdin, 1, SEEK_CUR);
		}
	}

	{register int t = 0;for(t;t<30;++t)printf("%d",t%10);putchar('\n');}

	{
		char** current = strings;
		do {
			printf("%15s %2d %10d\n",*current,strcc(*current,'t'),strsum(*current));
		} while((++current - strings) < 20);
	}

	return 0;

}
