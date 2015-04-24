#ifndef SORTANDSEARCHH
#define SORTANDSEARCHH

#include "db.h"

extern void bubbleSort(DBrecord *A[], int , Field);
extern int binarySearch(DBrecord *A[], DBrecord *, Field, int, int);
extern int compare(DBrecord *, DBrecord *, Field);
Boolean isGreater(DBrecord *, DBrecord *, Field);
void swap(DBrecord *A[], int, int);

#endif
