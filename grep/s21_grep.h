#ifndef S21_GREP_H_
#define S21_GREP_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <regex.h>

#define parametrsNumber 10
#define types {"e", "i", "v", "c", "l", "n", "h", "s", "file", "o"}

typedef struct {
    int param[10];
    int numberFile;
} s21_parametrs;

typedef struct name {
    char *value;
    struct name *next;
} namefile;

typedef struct list {
    char *value;
    struct list *next;
} stringlist;

namefile *s21_initialisation_file_name(void);
stringlist *s21_initialisation_string_list(void);

void s21_grep_parametrs_initialisation(s21_parametrs* parm);
int s21_grep_parametrs_read(s21_parametrs* parm, char** argv, int argc,
    namefile **fname, stringlist **strlist);
int s21_grep_parametrs_file_read(char** argv,
    namefile **fname, int count, int number);
int s21_grep_exists(const char *filename);

void s21_grep_error_print(char option, size_t number, char* argument);
void s21_grep_parametrs_string_read(char** argv,
    stringlist **strlist, int count, int number);
int s21_grep_search_lines(s21_parametrs* parm, char** argv, int argc,
    namefile *fname, stringlist **strlist, int *count);
int s21_grep_add_lines(namefile *fname, stringlist **strlistm);
void s21_grep_push(stringlist **strlist, char * dupString);
void s21_grep_file_count(s21_parametrs* parm, char** argv, int argc);
void s21_grep_search(FILE* grep, s21_parametrs* parm,
    stringlist **strlist, char* name);
int s21_grep_count_string(char *string_1, char * string_2);
void s21_grep_print_count_name(s21_parametrs* parm, char* temporaryLine,
    char* temporary, char* name, int stringCount);
void s21_grep_print_e(s21_parametrs* parm, char* temporaryLine,
    char* name, int stringCount);
void s21_grep_print_c_l(s21_parametrs *parm, char *name, int successCount);
void s21_grep_struct_free(namefile **fname,
    stringlist **strlist, int count);

#endif
