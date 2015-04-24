#define _GNU_SOURCE yes

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define sfclose(fd) if(fd) fclose(fd)

const char* HEADER = 	
            "+------------------------+\n"
			"| Roman Hargrave, 2015   |\n"
			"+------------------------+\n";

void encrypt_stream(FILE*, FILE*);
void decrypt_stream(FILE*, FILE*);

int main(int argc, char** argv) {

	printf(HEADER);

	const char* self = *argv++;

	if(argc < 4) {
		printf( "Both an input file and an output file must be specified at runtime\n"
			"Usage: %s <encrypt|decrypt> <input> <output> \n",
			self);
	       return EXIT_FAILURE;
	}

	const char* oper 	= *argv++;
	const char* inputFile 	= *argv++;
	const char* outputFile	= *argv++;

	if((strcmp(oper, "encrypt") != 0) & (strcmp(oper, "decrypt") != 0)) {
		printf( "Either the ``decrypt'' or ``encrypt'' operation must be specified, but %s was specified instead\n", oper );
		return EXIT_FAILURE;
	}


	FILE* inputStream	= NULL;
	FILE* outputStream	= NULL;

	
	if((strcmp(inputFile, "-") == 0)) {
		inputStream = stdin;
	} else {
		inputStream = fopen(inputFile, "r");
	}

	if((strcmp(outputFile, "-") == 0)) {
		outputStream = stdout;
	} else {
		outputStream = fopen(outputFile, "w");
	}

	if(!inputStream) {
		printf( "The specified input file, %s, does not exist\n", inputFile);
		sfclose(outputStream);
		return EXIT_FAILURE;
	}

	if(strcmp(oper, "encrypt") == 0) {
		encrypt_stream(inputStream, outputStream);	
	} else {
		decrypt_stream(inputStream, outputStream);
	}

	sfclose(inputStream);
	sfclose(outputStream);

	return EXIT_SUCCESS;
}

void encrypt_stream(FILE* inputStream, FILE* outputStream) {

//	const long filesize = fsize(inputStream);

	char intake = 0x00;
	do {
		intake = (char) fgetc(inputStream);
		fputc(~intake, outputStream);
		fputc(' ', outputStream);
	} while (intake != EOF);
}

void decrypt_stream(FILE* inputStream, FILE* outputStream) {

	char intake = 0x00;
	do {
		intake = (char) fgetc(inputStream);
		fgetc(inputStream);
		fputc(~intake, outputStream);
	} while (intake != EOF);
}
