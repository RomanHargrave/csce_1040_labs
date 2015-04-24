/* ******************************************************************** */
/*                                                                      */
/* Set.h --- this is the .h file associated with the (major 2) Set      */
/*           implementation used in Spring 2012's CSCE 1040 class.      */
/*                                                                      */
/*           This assignment includes 10 functions to be implemented    */
/*           in Set.c, namely                                           */
/*                                                                      */
/*                 clearSet                                             */
/*                 add2Set                                              */
/*                 deleteFromSet                                        */
/*                 isMember                                             */
/*                 printSet                                             */
/*                 setUnion                                             */
/*                 setIntersection                                      */
/*                                                                      */
/*  Author:  Philip Sweany                                              */
/*  No rights reserved                                                  */
/*                                                                      */
/* ******************************************************************** */
#ifndef SETH
#define SETH

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

// defining new type(s)
typedef unsigned int Set[10];

// declaring global variable(s)
                               

extern void setUnion(Set set1, Set set2, Set result);
extern void setIntersection(Set set1, Set set2, Set result);
extern void clearSet(Set set);
extern void add2Set(Set set, int value);
extern void deleteFromSet(Set set, int value);
extern int isMember(Set set, int element);
extern void copySet(Set dest, Set src);


extern void printSet(Set);		    // this time printSet should
					    // print the DBrecord rather 
					    // than just the integer values 
					    // of the bit position.

#endif
