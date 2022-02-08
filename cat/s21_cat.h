#ifndef S21_CAT_H_
#define S21_CAT_H_

#include <stdio.h>
#include <string.h>

#if defined(__APPLE__)
#define parametrsNumber 5
#define flags {"b", "e", "n", "s", "t"}

#elif defined(__linux__)
#define parametrsNumber 10
#define flags {"b", "e", "n", "s", "t", \
"--number-nonblank", "E", "--number", "--squeeze-blank", "T"}
#endif

typedef struct {
    int param[10];
    int number;
    int bufNumber;
} parametrs;

void s21_cat_parametrs_initialisation(parametrs* parametr);
int s21_cat_parametrs_read(parametrs* parametr,
    const char** argv, int argc);
int s21_cat_zero_some_parametrs(parametrs* parametr);
int s21_cat_print_char(parametrs* parametr,
    const char** argv, int argc);
int s21_cat_is_exists(const char *fileName);
void s21_cat_number_line(char presentChar,
    char futureChar, parametrs* parametr);
void s21_cat_squeeze_line(int presentChar,
    int* futureChar, parametrs* parametr, FILE *file);
void s21_cat_end_line(int futureChar, parametrs* parametr);

#endif
