#ifndef TABLE_PRINT_H
#define TABLE_PRINT_H


#include <stdio.h>
#include <stdarg.h>

#include "my402list.h"

#define MIN_COL_WIDTH 5
#define MAX_LEN 70

#ifdef __cplusplus

extern "C"{

#endif

extern FILE *output_file;
extern int NO_OF_HEADER;

extern int LEFT ;
extern int MID ;
extern int RIGHT ;



typedef struct TEXT{    
    char text[MAX_LEN];

}text;

typedef struct TEXT_PROP{
    int pos;
    int length;
}text_prop;




// set number of column in the table
extern void set_column_no (int);


// set the header name and what there size
extern void set_header (const char *, int , int );


// print the header box with content
extern void print_header();



// clear the flag and table
extern void clear_table(int);



// print row a table
extern void print_row (const char * , ...);


// get width
extern int get_width();
#ifdef __cplusplus
}
#endif

#endif

