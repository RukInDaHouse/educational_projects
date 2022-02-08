// Copyright mcheyk 2021

#include "s21_grep.h"

int main(int argc, char ** argv) {
    int count = 0;
    int errorParametr = 0;
    s21_parametrs parametr;
    namefile *fileName = s21_initialisation_file_name();
    stringlist *stringList = s21_initialisation_string_list();
    s21_grep_parametrs_initialisation(&parametr);
    errorParametr = s21_grep_parametrs_read(&parametr,
    argv, argc, &fileName, &stringList);
    if (errorParametr == 0) {
        errorParametr = s21_grep_search_lines(&parametr,
        argv, argc, fileName, &stringList, &count);
    }
    s21_grep_struct_free(&fileName, &stringList, count);
    return errorParametr;
}

namefile *s21_initialisation_file_name(void) {
    namefile *fileName =  malloc(sizeof(namefile));
    fileName -> next = NULL;
    return fileName;
}

stringlist *s21_initialisation_string_list(void) {
    stringlist *stringList =  malloc(sizeof(namefile));
    stringList -> next = NULL;
    return stringList;
}

void s21_grep_parametrs_initialisation(s21_parametrs* parametr) {
    int count;
    for (count = 0; count < 10; count++) {
        parametr -> param[count] = 0;
    }
    parametr -> numberFile = 0;
}

int s21_grep_parametrs_read(s21_parametrs* parametr, char** argv, int argc,
    namefile **fileName, stringlist **stringList) {
    static char *arr[] = types;
    int errorParametr = 0, numberString = 0, flag;
    int count_1, count_2, count_3;
    for (count_1 = 1; count_1 < argc && argc > 1; count_1++) {
        int lengthArgv = (int)strlen(argv[count_1]);
        if ((argv[count_1][0] == '-') && (lengthArgv > 1)) {
            flag = 0;
            for (count_2 = 1; count_2 < lengthArgv; count_2++) {
                for (count_3 = 0; count_3 < parametrsNumber; count_3++) {
                    if (strchr(arr[count_3], argv[count_1][count_2]) != NULL) {
                        parametr -> param[count_3]++;
                            if ((count_2 ==
                            (lengthArgv - 1))
                                && (count_1 < argc - 1) &&
                                ((count_3 == 8) || (count_3 == 0))) {
                                    if ((count_3 == 8)
                                        && (s21_grep_parametrs_file_read(argv,
                                        fileName, count_1, 0))) {
                                            errorParametr = 2;
                                            count_1 += argc;
                                    } else if (count_3
                                    == 0) {
                                        s21_grep_parametrs_string_read(argv,
                                        stringList, count_1, 0);
                                    }
                            } else if ((count_2 != (lengthArgv - 1))
                                && (count_1 < argc - 1) &&
                                (count_3 == 8 || count_3 == 0)) {
                                    if ((count_3 == 8)
                                        && (s21_grep_parametrs_file_read(argv,
                                        fileName, count_1, count_2))) {
                                            errorParametr = 2;
                                            count_1 += argc;
                                    } else if (count_3 == 0) {
                                        s21_grep_parametrs_string_read(argv,
                                        stringList, count_1, count_2);
                                    }
                                    count_2 += lengthArgv;
                            }
                        count_3 += parametrsNumber;
                        flag = 1;
                    }
                }
                if ((flag == 0) && (errorParametr == 0)) {
                    s21_grep_error_print(argv[count_1][count_2], 2, NULL);
                    errorParametr = 1;
                    break;
                }
                flag = 0;
            }
            argv[count_1] = "\0";
        } else if (numberString == 0) {
            numberString = count_1;
        }
    }
    if ((parametr -> param[0] == 0) && (parametr -> param[8] == 0)) {
        s21_grep_parametrs_string_read(argv, stringList, numberString - 1, 0);
    }
    return errorParametr;
}

int s21_grep_parametrs_file_read(char** argv,
namefile **fileName, int count, int number) {
    int errorParametr = 0;
    if (number == 0) {
        if (s21_grep_exists(argv[count + 1])) {
            namefile *temporary = malloc(sizeof(namefile));
            temporary -> value = argv[count + 1];
            temporary -> next = (*fileName);
            (*fileName) = temporary;
            argv[count+1] = "\0";
        } else {
            s21_grep_error_print(0, 1, argv[count + 1]);
            errorParametr = 2;
        }
    } else {
        int lenght = ((int)strlen(argv[count]) - number);
        char *stringTemporary = malloc(((long unsigned int)lenght + 1)*sizeof(char));
        for (int count_1 = 0; count_1 < lenght; count_1++) {
            stringTemporary[count_1] = argv[count][count_1 + number + 1];
        }
        stringTemporary[lenght] = '\0';
        snprintf(argv[count], sizeof(stringTemporary), "%s", stringTemporary);
        if (s21_grep_exists(argv[count])) {
            namefile *temporary = malloc(sizeof(namefile));
            temporary -> value = argv[count];
            temporary -> next = (*fileName);
            (*fileName) = temporary;
        } else {
            s21_grep_error_print(0, 1, argv[count]);
            errorParametr = 2;
        }
        free(stringTemporary);
    }
    return errorParametr;
}

int s21_grep_exists(const char *filename) {
    int returnValue = 0;
    FILE *file = fopen(filename, "r");
    if (file != NULL) {
        returnValue = 1;
        fclose(file);
    }
    return returnValue;
}

void s21_grep_error_print(char option, size_t number, char* argument) {
  if (number == 1) {
    fprintf(stderr, "grep: %s: %s\n", argument, strerror(2));
  } else if (number == 2) {
    fprintf(stderr,
            "grep: invalid option -- %c\nusage: grep "
            "[-abcDEFGHhIiJLlmnOoqRSsUVvwxZ] [-A number] [-B number] "
            "[-C[number]]\n\t[-e pattern] [-file file] [--binary-files=value] "
            "[--color=when]\n\t[--context[=number]] [--directories=action] "
            "[--label] [--line-buffered]\n\t[--null] [pattern] [file ...]\n",
            option);
  }
}

void s21_grep_parametrs_string_read(char** argv,
    stringlist **stringList, int count, int number) {
    if (number == 0) {
        stringlist *temporary = malloc(sizeof(namefile));
        temporary -> value = argv[count+1];
        temporary -> next = (*stringList);
        (*stringList) = temporary;
        argv[count+1] = "\0";
    } else {
        int lenght = ((int)strlen(argv[count]) - number);
        char *stringTemporary = malloc(((long unsigned int)lenght + 1)*sizeof(char));
        for (int count_1 = 0; count_1 < lenght; count_1++) {
            stringTemporary[count_1] = argv[count][count_1 + number + 1];
        }
        stringTemporary[lenght] = '\0';
        snprintf(argv[count], sizeof(stringTemporary), "%s", stringTemporary);
        stringlist *temporary = malloc(sizeof(stringlist));
        temporary -> value = argv[count];
        temporary -> next = (*stringList);
        (*stringList) = temporary;
        free(stringTemporary);
    }
}

int s21_grep_search_lines(s21_parametrs* parametr, char** argv, int argc,
    namefile *fileName, stringlist **stringList, int *count) {
    int errorParametr = 0;
    int count_1;
    if (parametr -> param[8] != 0) {
        *count = s21_grep_add_lines(fileName, stringList);
    }
    s21_grep_file_count(parametr, argv, argc);
    for (count_1 = 1; count_1 < argc; count_1++) {
        if ((argv[count_1][0] != '\0') && (s21_grep_exists(argv[count_1]))) {
            FILE* grep = fopen(argv[count_1], "r");
            s21_grep_search(grep, parametr, stringList, argv[count_1]);
            fclose(grep);
        } else if ((argv[count_1][0] != '\0') && (parametr -> param[7] == 0)) {
            s21_grep_error_print(0, 1, argv[count_1]);
            errorParametr = 2;
        }
    }
    return errorParametr;
}

int s21_grep_add_lines(namefile *fileName, stringlist **stringList) {
    int count = 0;
    size_t lenght = 0;
    if (fileName != NULL) {
        while ((fileName) -> next) {
        if (fileName -> value != NULL && s21_grep_exists(fileName -> value)) {
            ssize_t read;
            char* readString = NULL;
            FILE* file = fopen(fileName->value, "r");
            while ((read = getline(&readString, &lenght, file)) != -1) {
                s21_grep_push(stringList, readString);
                count++;
            }
            free(readString);
            fclose(file);
        }
            fileName = fileName -> next;
        }
    }
    return count;
}

void s21_grep_push(stringlist **stringList, char* dupString) {
    stringlist *temporary = malloc(sizeof(stringlist));
    temporary -> value = strdup(dupString);
    temporary -> next = (*stringList);
    (*stringList) = temporary;
}

void s21_grep_file_count(s21_parametrs* parametr, char** argv, int argc) {
  int count = 1;
  while (count != argc) {
    if (argv[count][0] != '\0') {
      parametr -> numberFile += 1;
    }
    count += 1;
  }
}

void s21_grep_search(FILE* grep,
s21_parametrs* parametr, stringlist **stringList, char* name) {
    stringlist *begin = *stringList;
    size_t lenght;
    ssize_t read = 0;
    int over = 0, readSuccessCount = 0, stringCount = 0, success;
    char* temporaryLine = NULL, *temporary = NULL;
    while (read != EOF && over != 1) {
        int repeat = 1;
        stringCount += 1;
        read = getline(&temporaryLine, &lenght, grep);
        regex_t regex;
        while ((*stringList) -> next) {
            temporary = (*stringList) -> value;
            if (parametr -> param[1] != 0) {
                regcomp(&regex, temporary, REG_ICASE);
            } else {
                regcomp(&regex, temporary, REG_EXTENDED);
            }
            success = regexec(&regex, temporaryLine, 0, NULL, 0);
            regfree(&regex);
            if ((success == 0) && (over == 0)) {
                if (parametr -> param[9] != 0) {
                    repeat = s21_grep_count_string(temporaryLine, temporary);
                }
                for (int count = 1; count <= repeat; count++) {
                    if ((parametr -> param[2] == 0)
                    && (parametr -> param[3] == 0)
                        && (parametr -> param[4] == 0)) {
                            s21_grep_print_count_name(parametr, temporaryLine,
                                temporary, name, stringCount);
                    }
                    over++;
                    if (parametr -> param[2] == 0) {
                        readSuccessCount++;
                    }
                }
            }
            (*stringList) = (*stringList) -> next;
        }
        if ((over == 0) && (parametr -> param[2] != 0) && (read != EOF)) {
            s21_grep_print_e(parametr, temporaryLine, name, stringCount);
            readSuccessCount++;
        }
        over = 0;
        *stringList = begin;
    }
    if (temporaryLine) {
        free(temporaryLine);
    }
    s21_grep_print_c_l(parametr, name, readSuccessCount);
}

int s21_grep_count_string(char *string_1, char * string_2) {
    int count = 0;
    char *ch = string_1;
    while ((ch = strstr(ch, string_2)) != NULL) {
        count++;
        ch += strlen(string_2);
    }
    return count;
}

void s21_grep_print_count_name(s21_parametrs* parametr, char* temporaryLine,
    char* temporary, char* name, int stringCount) {
    if ((parametr -> numberFile > 1) && (parametr -> param[6] == 0)) {
        printf("%s:", name);
    }
    if (parametr -> param[5] != 0) {
        printf("%d:", stringCount);
    }
    if (parametr -> param[9] == 0) {
        printf("%s", temporaryLine);
    } else {
        printf("%s\n", temporary);
    }
}

void s21_grep_print_e(s21_parametrs* parametr, char* temporaryLine,
    char* name, int stringCount) {
    if ((parametr -> param[3] == 0) && (parametr -> param[4] == 0)) {
        if ((parametr -> numberFile > 1) && (parametr -> param[6] == 0)) {
            printf("%s:", name);
        }
        if (parametr -> param[5] != 0) {
            printf("%d:", stringCount);
        }
        printf("%s", temporaryLine);
    }
}

void s21_grep_print_c_l(s21_parametrs *parametr, char *name, int successCount) {
    if (parametr -> param[3] != 0) {
        if ((parametr -> numberFile > 1) && (parametr -> param[6] == 0)) {
            printf("%s:", name);
        }
        printf("%d\n", successCount);
    }
    if ((parametr->param[4] != 0) && (successCount != 0)) {
        printf("%s\n", name);
    }
}

void s21_grep_struct_free(namefile **fileName,
    stringlist **stringList, int count) {
    namefile* preventDefault = NULL;
    if (*fileName != NULL) {
        while ((*fileName) -> next) {
            preventDefault = (*fileName);
            (*fileName) = (*fileName) -> next;
            free(preventDefault);
        }
        free(*fileName);
    }
    stringlist* preventDefault_1 = NULL;
    if (*stringList != NULL) {
        while ((*stringList) -> next) {
            preventDefault_1 = (*stringList);
            if (((*stringList) -> value != NULL) && (count > 0)) {
                count--;
                char *temporary = (*stringList) -> value;
                free(temporary);
            }
            (*stringList) = (*stringList) -> next;
            free(preventDefault_1);
        }
        free(*stringList);
    }
}
