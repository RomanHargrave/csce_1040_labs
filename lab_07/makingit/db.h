#ifndef DBH
#define DBH
typedef enum {firstYear, sophomore, junior, senior, grad} Class;
typedef enum {LastName=1, FirstName, ID, Age, Year, GPA, GradYear} Field;
typedef enum {False, True} Boolean;

typedef struct {
        int  bitID;
        char *lName;
        char *fName;
        char idNum[8];
        int age;
        Class year;
        float gpa;
        int gradYear;
    }DBrecord;

// functions
extern void printDBase(DBrecord *array[], int n);
extern void printDBRecord(DBrecord *);
extern int buildDataBase();

// global data

extern DBrecord ***theDataBase;

#endif
