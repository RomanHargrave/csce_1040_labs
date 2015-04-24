#include "defs.h"

DBrecord ***theDataBase;

char *convert2String(int y)
{
    char *result;
    result = (char *) malloc(10);
    switch(y)
    {
        case 0:  strcpy(result,"firstYear"); 
                 break;

        case 1:  strcpy(result,"sophomore"); 
                 break;

        case 2:  strcpy(result,"junior"); 
                 break;

        case 3:  strcpy(result,"senior"); 
                 break;

        case 4:  strcpy(result,"grad"); 
                 break;
  
        default:
        { 
	    printf("Error: Unexpected Class designation %d\n",y);
            assert(0);
        }
    }
}

void printDBrecord(DBrecord *dbr)
{
    int i;
    printf("%s, %s: %s %d %s %5.2f %d\n",
           dbr->lName, 
           dbr->fName, 
           dbr->idNum,
           dbr->age,
           convert2String(dbr->year),
           dbr->gpa,
           dbr->gradYear);
}

void printDBase(DBrecord *array[], int n)
{
    int i;
    for(i = 0; i < n; i++)
        printDBrecord(array[i]);
}

int buildDataBase()
{

     int i,j;
     int num;
     char temp1[257], temp2[257], classString[10];

     scanf("%d",&num);

     // dynamically allocate space for all 8 arrays of pointers to DBrecords
     theDataBase =  (DBrecord ***) malloc(8 * sizeof(DBrecord**));

     // NULL all pointers in all 8 arrays.
     for(i = 0; i < 8; i++)
     {
         theDataBase[i] = (DBrecord **) malloc(num * sizeof(DBrecord *));
         for(j = 0; j < num; j++)
 	     theDataBase[i][j] = NULL;
     }

     for(i = 0; i < num; i++)
     {
         theDataBase[0][i] = (DBrecord *) malloc (sizeof(DBrecord));
         theDataBase[0][i]->bitID = i;
         scanf("%s %s %s %d %s %f %d", temp1, temp2, theDataBase[0][i]->idNum, 
               &theDataBase[0][i]->age, classString, &theDataBase[0][i]->gpa, 
               &theDataBase[0][i]->gradYear);

         //  allocate space for character strings, and copy the strings */
         theDataBase[0][i]->lName = (char *) malloc(strlen(temp1) + 1);
         strcpy(theDataBase[0][i]->lName, temp1);
         theDataBase[0][i]->fName = (char *) malloc(strlen(temp2) + 1);
         strcpy(theDataBase[0][i]->fName, temp2);

        // set the year field from the strings of Class values
        if( (strcmp(classString,"firstYear")) == 0 )
	    theDataBase[0][i]->year = firstYear;

        else if( (strcmp(classString,"sophmore")) == 0 )
	    theDataBase[0][i]->year = sophomore;

        else if( (strcmp(classString,"junior")) == 0 )
	    theDataBase[0][i]->year = junior;

        else if( (strcmp(classString,"senior")) == 0 )
	    theDataBase[0][i]->year = senior;

        else if( (strcmp(classString,"grad")) == 0 )
	    theDataBase[0][i]->year = grad;

        else
	    assert(0);	// undefined Class value

        // With fields of this record set, copy to all arrays

         for( j = 1; j < 8; j++ )
	     theDataBase[j][i] = theDataBase[0][i];
     }
     // sort and print each sortable array.

     for( j = 1; j < 8; j++ )
     {
         bubbleSort(theDataBase[j], num, j);
         // printf("\tSorted by field %d\n",j);
         // printDBase(theDataBase[j], num);
         // printf("\n\n");
     }
     return 0; 
}
