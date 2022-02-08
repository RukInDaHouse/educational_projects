// Copyright school_21 2022
#include <check.h>
#include <stdio.h>

START_TEST(s21_cat_test) {
  FILE *s21_test = fopen("cat/s21_cat_result.txt", "r");
  FILE *cat_test = fopen("cat/cat_result.txt", "r");
  if (s21_test && cat_test) {
    char s21_cat_char = '\0', cat_char = '\0';
    while (cat_char != EOF && s21_cat_char != EOF) {
        s21_cat_char = getc(s21_test);
        cat_char = getc(cat_test);
      ck_assert_int_eq(s21_cat_char, cat_char);
    }
    fclose(s21_test);
    fclose(cat_test);
  }
}
END_TEST

START_TEST(s21_grep_test) {
  FILE *s21_test = fopen("grep/s21_grep_result.txt", "r");
  FILE *grep_test = fopen("grep/grep_result.txt", "r");
  if (s21_test && grep_test) {
    char s21_grep_char = '\0', grep_char = '\0';
    while (grep_char != EOF && s21_grep_char != EOF) {
        s21_grep_char = getc(s21_test);
        grep_char = getc(grep_test);
      ck_assert_int_eq(s21_grep_char, grep_char);
    }
    fclose(s21_test);
    fclose(grep_test);
  }
}
END_TEST

int main(void) {
    Suite *s21 = suite_create("s21_bash");
    TCase *s21_catCase = tcase_create("cat test");
    TCase *s21_grepCase = tcase_create("grep test");
    SRunner *ch = srunner_create(s21);
    int numFile;
    suite_add_tcase(s21, s21_catCase);
    tcase_add_test(s21_catCase, s21_cat_test);
    suite_add_tcase(s21, s21_grepCase);
    tcase_add_test(s21_grepCase, s21_grep_test);
    srunner_run_all(ch, CK_VERBOSE);
    numFile = srunner_ntests_failed(ch);
    srunner_free(ch);
    return numFile == 0 ? 0 : 1;
}
