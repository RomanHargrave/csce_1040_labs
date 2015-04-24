#include <stdio.h>
#include "prots.h"

/* Reads the data from a file and stores it in the array of structs */
void readData(struct student *db, int *size)
{
    char* file_name = "input";
    
    FILE *infile;
    infile=fopen(file_name,"r");
    
    int i = 0;
	int euid;
    fscanf(infile,"%d",&euid);
    while(euid > 0){
        db[i].euid = euid;
        fscanf(infile,"%s%s%f%d",db[i].fname,db[i].lname,&db[i].gpa,&euid);
        i++;
    }
    close(infile);
    *size = i;
}

/* get user input */
int getInput(struct student *db, int size)
{
    printf("db> ");
    char command[10];
    scanf("%s",command);
	if(strcmp(command,"quit")==0)
		return -1;
    else if(strcmp(command,"all")==0)
        printDatabase(db,size);
    else if(strcmp(command,"euid")==0)
        findRecordByEuid(db,size);
    else if(strcmp(command,"name")==0)
        findRecordByName(db,size);
    else if(strcmp(command,"lowest")==0)
        findRecordByLowestGpa(db,size);
    else if(strcmp(command,"highest")==0)
        findRecordByHighestGpa(db,size);
    else if(strcmp(command,"gpa")==0)
        findHighGpas(db,size);
    else
        printf("Command not supported\n");
	return 0;
}


/* Print hte menu */
void printMenu()
{
    printf("\n\n***************************************************\n");
    printf("Select one of the following options:\n");
    printf("all\t\t - print all records \n");
    printf("euid\t\t - search for euid \n");
    printf("name\t\t - search for last name \n");
    printf("lowest\t\t - print records with lowest GPA \n");
    printf("highest\t\t - print records with highest GPA \n");
    printf("gpa\t\t - print records with GPA >= 3.0 \n");
	printf("quit\t\t - exit the program \n");
    printf("***************************************************\n\n\n");
}


/* Print out the contents of the DB */
void printDatabase(struct student *db, int size)
{
    int i;
    for(i=0;i<=size;i++)
        printf("%d\t%s\t%s\t%.2f\n",db[i].euid,db[i].fname,db[i].lname,db[i].gpa);
}


/* Print out the record with a specific EUDI */
void findRecordByEuid(struct student *db, int size)
{
    int i;
    int id;
    scanf("%d", &id);
    for(i=0;i<size;i++) {
        if(db[i].euid==id) {
            printf("%d\t%s\t%s\t%.2f\n",db[i].euid,db[i].fname,db[i].lname,db[i].gpa);
            break; /*euid is unique, so stop here */
        } /* end if*/
    } /* end for */
}

/* Print out the record with a specific last name */
void findRecordByName(struct student *db, int size)
{
    int i;
    char name[100];
    scanf("%s", name);
    for(i=0;i<size;i++) {
        if(strcmp(db[i].lname,name)==0) {
            printf("%d\t%s\t%s\t%.2f\n",db[i].euid,db[i].fname,db[i].lname,db[i].gpa);
        } /* end if*/
    } /* end for */
}

/* Print the records with lowest GPA */
void findRecordByLowestGpa(struct student *db, int size)
{
    int i;
    float lowest=db[0].gpa;
    for(i=1;i<size;i++) {
        if(db[i].gpa<lowest) {
            lowest=db[i].gpa;
        }
    }
    for(i=0;i<size;i++) {
        if(db[i].gpa==lowest) {
            printf("%d\t%s\t%s\t%.2f\n",db[i].euid,db[i].fname,db[i].lname,db[i].gpa);
        }
    }
}

/* Print the records with highest GPA */
void findRecordByHighestGpa(struct student *db, int size)
{
    int i;
    float highest=db[0].gpa;
    for(i=1;i<size;i++) {
        if(db[i].gpa>highest) {
            highest=db[i].gpa;
        }
    }
    for(i=0;i<size;i++) {
        if(db[i].gpa==highest) {
            printf("%d\t%s\t%s\t%.2f\n",db[i].euid,db[i].fname,db[i].lname,db[i].gpa);
        }
    }
}

/* Print out all records with a GPA >= 3.0 */
void findHighGpas(struct student *db, int size)
{
    int i;
    for(i=0;i<size;i++) {
        if(db[i].gpa>=3.0) {
            printf("%d\t%s\t%s\t%.2f\n",db[i].euid,db[i].fname,db[i].lname,db[i].gpa);
        }
    }
}





