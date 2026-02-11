// generate a output tfile
#include "table_print.h"
#include <string.h>
#include <stdarg.h>
#include <unistd.h>

#include "my402list.h"

FILE *output_file = NULL;   // NULL : output will be in stdin
int NO_OF_HEADER = 0;
int width = 0;


int LEFT = 0;
int MID = 1;
int RIGHT =2;



My402List header_text;
My402List header_prop;

My402List row_text;

short flag;

#define HEADER_GEN 0x01
#define HEADER_PRINT 0x02

#define CLEAR_FLAG(x) ((x) = 0)

#define SET_HEADER_GEN(x) (x |= (HEADER_GEN))
#define SET_HEADER_PRINT(x) (x |= (HEADER_PRINT))

#define CLEAR_HEADER_GEN(x) (x &= (~HEADER_GEN))
#define CLEAR_HEADER_PRINT(x) (x &= (~HEADER_PRINT))

#define IS_HEADER_GEN(x) (((x & HEADER_GEN) == HEADER_GEN) ? 1 : 0)
#define IS_HEADER_PRINT(x) (((x & HEADER_PRINT) == HEADER_PRINT) ? 1 : 0)


// For free all connected data
void free_obj (void * obj){
    free(obj);
}



// set the no of column
void set_column_no (int noh){
    NO_OF_HEADER = noh;
    width += noh+1; // no of corner with extra corner
    My402ListInit(&header_text);
    My402ListInit(&header_prop);

    My402ListInit(&row_text);
}



// generated a perfect string with format
char* set_string_format(const char* str, int space)
{
    size_t str_len = strlen(str);
    char *result;

    if (space < (int)str_len + 2) {
        result = (char*)malloc(sizeof(char)*(space + 1));
        
        if (!result) return NULL;

        memset(result, 0, space + 1);
        
        result[0] = ' ';
        strncat(result, str, space - 5);
        strcat(result, "...");
        
        result[space - 1] = ' ';
        result[space] = '\0';
    } else {

        result = (char*)malloc(sizeof(char)*(str_len + 3));

        if (!result) return NULL;

        memset(result, 0, sizeof(char) * (str_len + 3));
        result[0] = ' ';

        strncat(result, str,str_len);
        
        result[str_len + 1] = ' ';
        result[str_len + 2] = '\0';
        
    }

    return result;   
}


// set a header-column of a table
void set_header (const char *name, int pos, int lenght){
    if (IS_HEADER_PRINT(flag)==1){
        fprintf(stderr,"Already header is printed.");
        exit(-1);
    }

    text * t = (text*) malloc (sizeof(text));
    text_prop *tp = (text_prop *) malloc (sizeof(text_prop));

    if (t == NULL || tp == NULL) exit(-1);
    
    memset(t,0,sizeof(text));
    memset(tp,0,sizeof(text_prop));

    strncpy(t->text,name,strlen (name));


    tp->pos = pos;
    tp->length = lenght > MIN_COL_WIDTH ? lenght : MIN_COL_WIDTH;

    width+=lenght;

    My402ListAppend(&header_text,(void *)t);
    My402ListAppend(&header_prop,(void *)tp);
    
    SET_HEADER_GEN(flag);


}


// print width border of a table
void print_border_line(){

    // char border[width + 1 ];
    // memset(border,0,width+1);

    int col_controller = 0;

    if (IS_HEADER_GEN(flag)==0){
        fprintf(stderr,"Header is not generated.");
        exit (-1);
    }
    

    for (My402ListElem * elem = My402ListFirst(&header_prop);
        elem!= NULL && col_controller < NO_OF_HEADER;
        elem = My402ListNext(&header_prop,elem) , col_controller ++
        ){
    
            // strcat(border,"+");
            printf("+");
            text_prop* tp = (text_prop*)elem->obj;
            for (int i = 0; i < tp->length ; i++){
                // strcat(border,"-");

                printf("-");
                // usleep(10000);
                // fflush(stdout);
            }
        }
        printf("+");
        printf("\n");
}



// print a cell value with perfect allignment
void print_elem (text * t , text_prop *tp){
   

    char *format_str = set_string_format(t->text,tp->length);

    if (tp->pos == RIGHT){

        printf("|%*s",tp->length,format_str);

    }else if (tp->pos == MID){

        printf("|%*s%s%*s",
       ((int)((tp->length- (int)strlen(format_str)) / 2))  , "",
         format_str,
       ((int)((tp->length - (int)strlen(format_str) +1 ) / 2)) , "");


    }else{
        printf("|%*s",(-(tp->length)),format_str);

    }
    // usleep(10000);
    // fflush(stdout);
    

    free(format_str);

}



// when all header set then print the header
void print_header(){

    if (IS_HEADER_GEN(flag)==0){
        fprintf(stderr,"Header is not generated.");
        exit (-1);
    }

    print_border_line();
    for (My402ListElem *t = My402ListFirst(&header_text), *tp = My402ListFirst(&header_prop);
            t != NULL && tp != NULL;
            t = My402ListNext(&header_text,t),tp = My402ListNext(&header_prop,tp)
        ){
            print_elem((text*)(t->obj),(text_prop*)(tp->obj));
        }


    printf("|\n");

    print_border_line();

    SET_HEADER_PRINT(flag);

}

// truncate the string of the table size


// print each row
void print_row (const char *col1 , ...){

    if(IS_HEADER_PRINT(flag)==0){
        fprintf(stderr,"No Header is print.");
        exit(-1);
    }


    text *col;

    va_list cols;
    va_start (cols,col1);

    col = (text *)malloc (sizeof(text));
    if(col==NULL) exit(-1);

    memset(col,0,sizeof (text));
    strncpy(col->text,col1,strlen(col1));

    My402ListAppend(&row_text,(void*)col);

    for (int i = 1 ; i < NO_OF_HEADER ; i++ ){
        col = (text *)malloc (sizeof(text));
        if(col==NULL) exit(-1);

        memset(col,0,sizeof (text));
        char *va_col = va_arg(cols,char*);
        
        if (MAX_LEN>strlen(va_col))
            strncpy(col->text,va_col,strlen(va_col));
        else   
            strncpy(col->text,va_col,MAX_LEN);

        My402ListAppend(&row_text,(void*)col);
    }
    va_end(cols);



    for (My402ListElem *t = My402ListFirst(&row_text), *tp = My402ListFirst(&header_prop);
            t != NULL && tp != NULL;
            t = My402ListNext(&row_text,t),tp = My402ListNext(&header_prop,tp)
        ){
            print_elem((text*)(t->obj),(text_prop*)(tp->obj));
        }
    printf("|\n");

    My402ListForEach(&row_text,free_obj);
    My402ListUnlinkAll(&row_text);
}


// clear the table and close the border
void clear_table (){
    print_border_line();

    CLEAR_FLAG(flag);
    My402ListForEach(&header_text,free_obj);
    My402ListUnlinkAll(&header_text);

    My402ListForEach(&header_prop,free_obj);
    My402ListUnlinkAll(&header_prop);


    NO_OF_HEADER = 0;
    width = 0;
}

int get_width(){
    return width;
}





