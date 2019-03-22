#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gmp.h>

#define BUFFER_SZ 1024

static int check(const char * x, mpz_t * acc)
{
    static char buffer[BUFFER_SZ];
    int iteration = 0;
    size_t len = 0;
    strncpy(buffer, x, BUFFER_SZ);
    for (iteration = 0, len = strlen(x); len > 1; iteration++) {
        int i = 0;
        mpz_set_si(*acc, 1);
        for (i = 0; i < len; i++) {
            mpz_mul_si(*acc, *acc, buffer[i] - '0');
        }
        mpz_get_str(buffer, 10, *acc);
        len = strlen(buffer);
    }
    return iteration;
}

#ifdef GEN_RECURSIVE
static void generate(char * buffer, int digits, int d, char min)
{
    char i = 0;
    if (d >= digits) {
        buffer[d] = '\0';
        printf("Terminating recursion: %s\n", buffer);
        return;
    }
    for (i = min; i <= '9'; i++) {
        buffer[d] = i;
        generate(buffer, digits, d + 1, i);
    }
}
#else
static void generate(char * buffer, int digits, int d, char min)
{
    char i = 0;
    while (1) {
        if (d >= digits) {
            buffer[d] = '\0';
            printf("Terminating recursion: %s\n", buffer);
            do {
                d -= 1;
                if (d < 0) {
                    return;
                }
                min = buffer[d] + 1;
            } while (min > '9');
        }
        for (i = min; i <= '9'; i++) {
            buffer[d] = i;
            d += 1;
            min = i;
            break;
        }
    }
}
#endif

/**
 * Pairs not to include for efficiency:
 * 2x2=4
 * 2x3=6
 * 2x4=8
 * 3x2=6
 * 3x3=9
 * 4x2=8
 */
int main(int argc, char * argv[])
{
    char buffer[BUFFER_SZ];
    mpz_t acc;
    int p = 0;
    const char * test = "277777788888899";
    if (3 != argc) {
        printf("Usage: %s <seed> <count>\n", argv[0]);
        return 1;
    }
    mpz_init(acc);
    generate(buffer, 6, 0, '0');
    p = check(test, &acc);
    printf("Persistence of %s is %d\n", test, p);
    mpz_clear(acc);
    return 0;
}
