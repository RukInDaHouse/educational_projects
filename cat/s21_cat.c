// Copyright mcheyk 2021

#include "s21_cat.h"

int main(int argc, const char ** argv) {
    parametrs parametr;
    s21_cat_parametrs_initialisation(&parametr);
    int errorParametr = s21_cat_parametrs_read(&parametr, argv, argc);
    if (errorParametr == 0) {
        s21_cat_zero_some_parametrs(&parametr);
        errorParametr = s21_cat_print_char(&parametr, argv, argc);
    }
    return errorParametr;
}

void s21_cat_parametrs_initialisation(parametrs* parametr) {
    int count;
    for (count = 0; count < 10; count++) {
        parametr -> param[count] = 0;
    }
    parametr -> number = 1;
    parametr -> bufNumber = 0;
}

int s21_cat_parametrs_read(parametrs* parametr, const char** argv, int argc) {
    static char *array[] = flags;
    int errorParametr = 0, flag = 0;;
    int count_1, count_2, count_3;
    for (count_1 = 1; count_1 < argc && argc > 1; count_1++) {
        int lengthArgv = (int)strlen(argv[count_1]);
        if ((argv[count_1][0] == '-') &&
        (lengthArgv > 1) && (argv[count_1][1] != '-')) {
            for (count_2 = 1; count_2 < lengthArgv; count_2++) {
                for (count_3 = 0;
                count_3 < parametrsNumber; count_3++) {
                    if (strchr(array[count_3],
                    argv[count_1][count_2]) != NULL) {
                        parametr -> param[count_3]++;
                        count_3 += parametrsNumber;
                        flag = 1;
                    }
                }
                if ((flag == 0) && (errorParametr == 0)) {
                    fprintf(stderr,
"cat: invalid option -- '%c'\nTry 'cat --help' for more information.\n",
                        argv[count_1][count_2]);
                    errorParametr = 1;
                    break;
                }
                flag = 0;
            }
            argv[count_1] = "\0";
        } else if ((argv[count_1][0] == '-')
        && (lengthArgv > 1) && (argv[count_1][1] == '-')) {
            for (count_3 = 5; count_3 < parametrsNumber; count_3++) {
                if (strstr(argv[count_1], array[count_3]) != NULL) {
                    parametr -> param[count_3]++;
                    count_3 += parametrsNumber;
                    flag = 1;
                }
            }
            if ((flag == 0) &&
            (errorParametr == 0)) {
                fprintf(stderr,
"cat: unrecognized option '%s'\nTry 'cat --help' for more information.\n",
                        argv[count_1]);
                errorParametr = 1;
                break;
            }
            argv[count_1] = "\0";
        }
    }
    return errorParametr;
}

int s21_cat_zero_some_parametrs(parametrs* parametr) {
    if (parametr -> param[0] != 0) {
        parametr -> param[2] = 0;
    }
    if (parametr -> param[1] != 0) {
        parametr -> param[6] = 0;
    }
    if (parametr -> param[4] != 0) {
        parametr -> param[9] = 0;
    }
    if (parametr -> param[5] != 0) {
        parametr -> param[0] = 1;
        parametr -> param[5] = 0;
    }
    if (parametr -> param[7] != 0) {
        parametr -> param[2] = 1;
        parametr -> param[7] = 0;
    }
    if (parametr -> param[8] != 0) {
        parametr -> param[3] = 1;
        parametr -> param[8] = 0;
    }
    return 0;
}

int s21_cat_print_char(parametrs* parametr, const char** argv, int argc) {
    int errorParametr = 0, presentChar, futureChar;
    int count;
    for (count = 1; count < argc; count++) {
#if defined(__APPLE__)
        parametr -> number = 1;
#endif
        if (argv[count][0] != '\0') {
            if (s21_cat_is_exists(argv[count])) {
                FILE *file = fopen(argv[count], "r");
                parametr -> bufNumber = 0;
                presentChar = fgetc(file);
                futureChar = fgetc(file);
                s21_cat_number_line(
                    (char)presentChar, (char)futureChar, parametr);
                while (futureChar != EOF) {
                    s21_cat_squeeze_line(
                        presentChar, &futureChar, parametr, file);
                    if ((presentChar == '\t') && (parametr -> param[4] != 0)) {
                        printf("^I");
                    } else if ((presentChar >= 0) && (presentChar < 32) &&
                        (presentChar != '\n') && (presentChar != '\t')
                            && ((parametr -> param[1] != 0)
                            || (parametr -> param[4] != 0))) {
                                printf("^%c", presentChar + 64);
                    } else {
                        printf("%c", presentChar);
                    }
                    s21_cat_number_line((char)presentChar, (char)futureChar, parametr);
                    s21_cat_end_line(futureChar, parametr);
                    presentChar = futureChar;
                    futureChar = fgetc(file);
                }
                if (presentChar != EOF) {
                  putchar(presentChar);
                }
                fclose(file);
            } else {
            printf("cat: can't open '%s': No such file or directory\n", argv[count]);
                errorParametr = 1;
            }
        }
    }
    return errorParametr;
}

int s21_cat_is_exists(const char *fileName) {
    int returnValue = 0;
    FILE *file = fopen(fileName, "r");
    if (file != NULL) {
        returnValue = 1;
        fclose(file);
    }
    return returnValue;
}

void s21_cat_number_line(char presentChar,
char futureChar, parametrs* parametr) {
    if (((presentChar == '\n') || (parametr -> bufNumber == 0))
        && (futureChar != EOF)) {
            if (parametr -> param[2] != 0 || (parametr -> param[0] != 0 &&
                ((futureChar != '\n' && presentChar == '\n')
                    || (presentChar != '\n' && parametr -> bufNumber == 0)))) {
                        printf("%6d\t", parametr -> number);
                        parametr -> number += 1;
            }
    }
    parametr -> bufNumber += 1;
}

void s21_cat_squeeze_line(int presentChar,
int* futureChar, parametrs* parametr, FILE *file) {
    if (presentChar == '\n'
    && *futureChar == '\n' && parametr -> param[3] != 0) {
        while (*futureChar == '\n') {
            *futureChar = fgetc(file);
        }
        if ((parametr -> bufNumber == 0) && (*futureChar != EOF)) {
            if (parametr -> param[2] != 0) {
                printf("\n%6d\t", parametr -> number);
                parametr -> number += 1;
            } else {
                printf("\n");
            }
        } else {
            printf("\n");
            if (parametr -> param[2] != 0 && parametr -> param[0] == 0) {
                printf("%6d\t", parametr -> number);
                parametr -> number += 1;
            }
        }
        if (parametr -> param[1] != 0 || parametr -> param[6] != 0) {
            printf("$");
        }
    }
}

void s21_cat_end_line(int futureChar, parametrs* parametr) {
    if ((futureChar == '\n') &&
    ((parametr -> param[1] != 0) || (parametr -> param[6] != 0))) {
        printf("$");
    }
}
