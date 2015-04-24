#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include "course/student.h"
#include "course/bubble.h"

#define sfree(ptr) if(ptr) free(ptr)
#define snfree(ptr, n) { size_t i = 0; while(i < n) sfree(ptr+i++); }

#define min(x, y) ((x < y) * x ?: y)
#define max(x, y) ((x > y) * x ?: y)

static const char* STUDENT_FORMAT = "%9s %9s %u %u %u";

static const size_t STUDENT_COUNT = 19;

static const unsigned int COL_PADDING = 4;

typedef unsigned char ubyte;

typedef struct S_CourseInfo {

    float mean;
    float min;
    float max;
    float median;

    char* name;

} CourseInfo;

inline float Student_mean(student* student) {
    return (student->exam1 + student->exam2 + student->exam3) / 3.f;
}

void String_trim(char* string) {

    size_t initial = strlen(string);

    if(initial <= 0) return;

    char* seek = string;

    while(isspace(*seek)) ++seek;

    printf(seek); putc('\n', stdout);

    strncpy(string, seek, strlen(seek));

    size_t pos;
    for(pos = strlen(string) - 1; isspace(string[pos]); --pos) {
    }
}

void String_alignInSpace(const char* string, const size_t width, char destination[]) {

    size_t strLength = strlen(string);

    if (strLength >= width) {
        strncpy(destination, string, width);
        memset(destination + width, 0x00, 1);
        return;
    }

    size_t shift = width - strLength;

    char filler[shift];
    memset(filler, ' ', shift - 1);
    filler[shift] = 0x00;

    snprintf(destination, width, "%s%s", string, filler);
}

void Table_allocStrings(size_t nrows, size_t ncolumns, char* rows[nrows][ncolumns], size_t strlen) {
    size_t rowidx, colidx;
    for (rowidx = 0; rowidx < nrows; ++rowidx) {
        for (colidx = 0; colidx < ncolumns; ++colidx) {
            rows[rowidx][colidx] = calloc(sizeof(char), strlen);
        }
    }
}

void Table_unallocStrings(size_t nrows, size_t ncolumns, char* rows[nrows][ncolumns]) {
    size_t rowidx, colidx;
    for (rowidx = 0; rowidx < nrows; ++rowidx) {
        for (colidx = 0; colidx < ncolumns; ++colidx) {
            free(rows[rowidx][colidx]);
        }
    }
}

void Table_printRows(FILE* stream, size_t nColumns, size_t nRows, const char* columns[], const char* rows[][nColumns]) {

    // Calculate widest column -----------------------------------------------------------------------------------------
    size_t columnWidth[nColumns];

    size_t idx, rowIdx, colIdx;

    // Populate initial column sizes
    for (idx = 0; idx < nColumns; ++idx) {
        columnWidth[idx] = strlen(columns[idx]) + (COL_PADDING * (idx != (nColumns - 1)));
    }

    for (rowIdx = 0; rowIdx < nRows; ++rowIdx) {
        for (colIdx = 0; colIdx < nColumns; ++colIdx) {
            size_t length = strlen(rows[rowIdx][colIdx]) + (COL_PADDING * (colIdx != (nColumns - 1)));
            columnWidth[colIdx] = (length > columnWidth[colIdx]) ? length : columnWidth[colIdx];
        }
    }

    size_t widthTotal = 0;
    for (idx = 0; idx < nColumns; ++idx) {
        widthTotal += columnWidth[idx];
    }

    // Format header ---------------------------------------------------------------------------------------------------

    for (idx = 0; idx < nColumns; ++idx) {
        char colBuff[columnWidth[idx]];
        String_alignInSpace(columns[idx], columnWidth[idx], colBuff);
        fputs(colBuff, stream);
    }

    fputs("\n", stream);

    for (idx = 0; idx < widthTotal; ++idx) {
        fputc('-', stream);
    }

    fputs("\n", stream);

    for (rowIdx = 0; rowIdx < nRows; ++rowIdx) {
        for (colIdx = 0; colIdx < nColumns; ++colIdx) {
            char colBuff[columnWidth[colIdx]];
            String_alignInSpace(rows[rowIdx][colIdx], columnWidth[colIdx], colBuff);
            fputs(colBuff, stream);
        }
        fputs("\n", stream);
    }

}

inline float Students_mean(student** students, const size_t nStudents) {

    float mean = 0;
    student** current = students;

    while ((current - students) < nStudents) mean += (*current++)->mean;

    return mean / nStudents;
}

inline float Students_gradeMin(student** students, const size_t nStudents) {

    student** current = students;
    float min = (*current++)->mean;

    while ((current - students) < nStudents) min = min(min, (*current++)->mean);

    return min;
}

inline float Students_gradeMax(student** students, const size_t nStudents) {

    student** current = students;
    float max = (*current++)->mean;

    while ((current - students) < nStudents) max = max(max, (*current++)->mean);

    return max;
}


int main(void) {

    FILE* input =
#ifdef __DEBUG
            fopen("course/grades", "r");
#else
        stdin;
#endif

    char* currentLine = calloc(sizeof(char), 255);
    student* students[STUDENT_COUNT];
    student** current = students;
    CourseInfo course = { };

    size_t lineSize = 255;

    while ((getline(&currentLine, &lineSize, input) > 0)) {

        student workingStudent = { };

        workingStudent.first    = calloc(sizeof(char), 10);
        workingStudent.last     = calloc(sizeof(char), 10);

        int fieldCount =
                sscanf(currentLine, STUDENT_FORMAT, workingStudent.first, workingStudent.last, &workingStudent.exam1,
                       &workingStudent.exam2, &workingStudent.exam3);



        if (fieldCount < 5) {
            course.name = calloc(sizeof(char), strlen(currentLine));
            strcpy(course.name, currentLine);
            String_trim(course.name);
        } else {

            String_trim(workingStudent.first);
            String_trim(workingStudent.last);

            workingStudent.mean = Student_mean(&workingStudent);

            *current = malloc(sizeof(workingStudent));
            (*current)->first = malloc(sizeof(char) * 10);
            (*current)->last = malloc(sizeof(char) * 10);

            memcpy(current, &workingStudent, sizeof(workingStudent));

            (*current)->first = workingStudent.first;
            (*current)->last  = workingStudent.last;

            printf(STUDENT_FORMAT, workingStudent.first, workingStudent.last, workingStudent.exam1,
                   workingStudent.exam2, workingStudent.exam3);
            printf("\n");

            ++current;
        }

    }

#ifdef __DEBUG
    fclose(input);
#endif

    sfree(currentLine);

    bubble(students, STUDENT_COUNT);

    course.mean = Students_mean(students, STUDENT_COUNT);
    course.max = Students_gradeMax(students, STUDENT_COUNT);
    course.min = Students_gradeMin(students, STUDENT_COUNT);

    ubyte idx = 0;
    while (idx < 60) printf("%u", (idx++) % 10);
    printf("\n");

    printf("%s MEAN: %02.00f MIN: %02.00f MAX: %02.00f MEDIAN: %02.00f\n", course.name, course.mean, course.min,
           course.max, course.median);

    char* table[STUDENT_COUNT][3];

    Table_allocStrings(STUDENT_COUNT, 3, table, 255);

    char* (* row)[3] = table;

    while ((row - table) < STUDENT_COUNT) {

        strcpy(*row[0], students[row - table]->first);
        strcpy(*row[1], students[row - table]->last);
        sprintf(*row[2], "%02.00f", students[row - table]->mean);

        ++row;
    }

    Table_printRows(stdout, 3, STUDENT_COUNT, (const char* []) { "First", "Last", "Average" },
                    table);

    Table_unallocStrings(STUDENT_COUNT, 3, table);

    snfree(students, STUDENT_COUNT);
    sfree(course.name);

    if (errno == EINVAL) {
        printf("Unable to read from stdin\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
