OS := $(shell uname -s)
CC = gcc -std=c11
CFLAGS = -Wall -Wextra -Werror -c
EXTRA_CFLAGS = 
#-fdata-sections -ffunction-sections -Wconversion -Waggregate-return -Wpointer-arith -Wstrict-prototypes -Wundef -pedantic

all: clean s21_cat s21_grep

ifeq ($(UNAME),Linux)
	ADD_LIB = -lm -lrt -lpthread
endif

test: clean test_cat test_grep
ifeq ($(OS), Darwin)
	$(CC) s21_test.c -o s21_test -I$(HOME)/.brew/include -L$(HOME)/.brew/Cellar/check/0.15.2/lib -lcheck
		./s21_test	
else
	$(CC) s21_test.c -o s21_test -lcheck -lm -lrt -lpthread -lsubunit
		./s21_test		
endif

test_cat: clean_texts s21_cat
	@cat/s21_cat -e cat/Tests/empty_test.txt cat/Tests/common_text_test.txt cat/Tests/many_symbols.txt >> cat/s21_cat_after_test_result.txt
	@cat -e cat/Tests/empty_test.txt cat/Tests/common_text_test.txt cat/Tests/many_symbols.txt >> cat/cat_after_test_result.txt
	@cat/s21_cat -b cat/Tests/empty_test.txt cat/Tests/common_text_test.txt cat/Tests/many_symbols.txt  >> cat/s21_cat_after_test_result.txt
	@cat -b cat/Tests/empty_test.txt cat/Tests/common_text_test.txt cat/Tests/many_symbols.txt  >> cat/cat_after_test_result.txt
	@cat/s21_cat -n cat/Tests/empty_test.txt cat/Tests/common_text_test.txt cat/Tests/many_symbols.txt >> cat/s21_cat_after_test_result.txt
	@cat -n cat/Tests/empty_test.txt cat/Tests/common_text_test.txt cat/Tests/many_symbols.txt >> cat/cat_after_test_result.txt
	@cat/s21_cat -s cat/Tests/empty_test.txt cat/Tests/common_text_test.txt cat/Tests/many_symbols.txt >> cat/s21_cat_after_test_result.txt
	@cat -s cat/Tests/empty_test.txt cat/Tests/common_text_test.txt cat/Tests/many_symbols.txt  >> cat/cat_after_test_result.txt
	@cat/s21_cat -t cat/Tests/empty_test.txt cat/Tests/common_text_test.txt cat/Tests/many_symbols.txt   >> cat/s21_cat_after_test_result.txt
	@cat -t cat/Tests/empty_test.txt cat/Tests/common_text_test.txt cat/Tests/many_symbols.txt  >> cat/cat_after_test_result.txt
	@cat/s21_cat -benst cat/Tests/common_text_test.txt >> cat/s21_cat_after_test_result.txt
	@cat -benst cat/Tests/common_text_test.txt >> cat/cat_after_test_result.txt
	
test_grep: clean_texts s21_grep
	@grep/s21_grep -e w grep/Tests/common_text_test.txt >> grep/s21_grep_result.txt
	@grep -e w grep/Tests/common_text_test.txt >> grep/grep_result.txt
	@grep/s21_grep -i w grep/Tests/common_text_test.txt >> grep/s21_grep_result.txt
	@grep -i w grep/Tests/common_text_test.txt >> grep/grep_result.txt
	@grep/s21_grep -v w grep/Tests/common_text_test.txt >> grep/s21_grep_result.txt
	@grep -v w grep/Tests/common_text_test.txt >> grep/grep_result.txt
	@grep/s21_grep -c w grep/Tests/common_text_test.txt >> grep/s21_grep_result.txt
	@grep -c w grep/Tests/common_text_test.txt >> grep/grep_result.txt
	@grep/s21_grep -l w grep/Tests/common_text_test.txt >> grep/s21_grep_result.txt
	@grep -l w grep/Tests/common_text_test.txt >> grep/grep_result.txt
	@grep/s21_grep -n w grep/Tests/common_text_test.txt >> grep/s21_grep_result.txt
	@grep -n w grep/Tests/common_text_test.txt >> grep/grep_result.txt
	@grep/s21_grep -h w grep/Tests/common_text_test.txt grep/Tests/common_text_test.txt >> grep/s21_grep_result.txt
	@grep -h w grep/Tests/common_text_test.txt grep/Tests/common_text_test.txt >> grep/grep_result.txt
	@grep -f grep/Tests/many_symbols.txt grep/Tests/common_text_test.txt >> grep/grep_result.txt
	@grep/s21_grep -f grep/Tests/many_symbols.txt grep/Tests/common_text_test.txt >> grep/s21_grep_result.txt
	@grep -o w grep/Tests/common_text_test.txt >> grep/grep_result.txt
	@grep/s21_grep -o w grep/Tests/common_text_test.txt >> grep/s21_grep_result.txt
	@grep/s21_grep -f grep/Tests/many_symbols.txt grep/Tests/common_text_test.txt -nc >> grep/s21_grep_result.txt
	@grep -f grep/Tests/many_symbols.txt grep/Tests/common_text_test.txt -nc >> grep/grep_result.txt
	

s21_cat:
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) cat/s21_cat.c -o cat/s21_cat.o
	$(CC) cat/s21_cat.o -o cat/s21_cat

s21_grep:
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) grep/s21_grep.c -o grep/s21_grep.o
	$(CC) grep/s21_grep.o -o grep/s21_grep

clean: clean_texts
	rm -rf cat/*.o *.o cat/s21_cat s21_test
	rm -rf grep/*.o *.o grep/s21_grep
	rm -rf *.cfg

clean_texts:
	rm -rf cat/*_result.txt
	rm -rf grep/*_result.txt

rebuild: clean all

linters:
	``python3 ../materials/linters/cpplint.py --extensions=c ./cat/*.c``
	``python3 ../materials/linters/cpplint.py --extensions=c ./grep/*.c``

