#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define TEST_FUNCTION fabs
#define SOURCE_FUNCTION fabs

long double rand_range(long double min, long double max);

int main(void) {
    long double random_number = 0;
    for (int i = 0; i < 5000; i++) {
        random_number = rand_range(-5000,5000);
        if (TEST_FUNCTION(random_number) != SOURCE_FUNCTION(random_number)) {
            printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
            printf("%f\n", TEST_FUNCTION(random_number));
            printf("%f\n", SOURCE_FUNCTION(random_number));
            printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n\n");
        }
    }
  return 0;
}

long double rand_range(long double min, long double max) {
    long double random = ((double) rand()) / RAND_MAX;
    long double range = (max - min) * random;
    long double number = min + range;
    return number;
}
