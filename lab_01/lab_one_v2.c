#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "student.h"
#include "bubble.h"

static const char* INPUT_FORMAT = "%s %s %d %d %d";

int main(void) {

	student* students[19];
	char* className;

	{
		FILE* input = stdin;
		student** current = students;

		{
			char nameLocal[512];
			fgets(nameLocal, 511, input);
			if(strlen(nameLocal) > 2) nameLocal[strlen(nameLocal) - 2] = '\0';
			asprintf(&className, nameLocal);
		}

		while((current - students) < 19) {
			student new;
			if(fscanf(input, INPUT_FORMAT, new.first, new.last, &new.exam1, &new.exam2, &new.exam3) > 0) {
				new.mean = ((float)(new.exam1 + new.exam2 + new.exam3))/(3.0F);
				student* perst = (student*) malloc(sizeof(student));
				memcpy(perst, &new, sizeof(student));
				*current++ = perst;
			}
		}

		if(input != stdin) fclose(input);
	}


	bubble(students, 19);

	{
		float classMax = students[18]->mean;
		float classMin = students[0]->mean;
		float classMed = students[9]->mean;
		float classMean;
		{
			float classTotal;
			student** current = students;
			while((current - students) < 19) {
				classTotal += (*current)->mean;
				++current;
			}
			classMean = classTotal / 19.0F;
		}

		printf("%s MEAN: %3.02f MIN: %3.02f MAX: %3.02f MEDIAN: %3.02f \n", 
				className, classMean, classMin, classMax, classMed);

		{
			student** current = students;
			while((current - students) < 19) {
				student* c = *current;
				printf("%10s %10s %2.02f\n", c->first, c->last, c->mean);
				++current;
			}
		}
	}

	return 0;	
}

/* vim: let b:syntastic_c_cflags="-Wpedantic":
 */
