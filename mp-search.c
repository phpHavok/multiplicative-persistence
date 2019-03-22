#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gmp.h>

/**
 * The largest supported integer may have (BUFFER_SZ - 1) digits.
 */
#define BUFFER_SZ 1024

/**
 * Application state: stored globally so that it can be dumped on exit.
 */
static char buffer[BUFFER_SZ];
static int digits = 0;
static int d = 0;
static char min = '0';
static mpz_t acc;

/**
 * Generator callback function pointer which can check generated integers.
 */
typedef int (*generator_cb)(const char *, void *);

/**
 * Return the multiplicative persistence of the integer represented in base 10
 * by the string x. This function requires an accumulator to be initialized
 * already.
 */
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

/**
 * Generate candidate integers iteratively.
 *
 * Pairs not to include for efficiency:
 * 2x2=4
 * 2x3=6
 * 2x4=8
 * 3x2=6
 * 3x3=9
 * 4x2=8
 */
static void generate(char * buffer, int * digits, int * d, char * min,
        generator_cb cb, void * data)
{
    char i = 0;
    while (1) {
        if (*d >= *digits) {
            buffer[*d] = '\0';
            if (0 != cb(buffer, data)) {
                exit(2);
            }
            do {
                *d -= 1;
                if (*d < 0) {
                    return;
                }
                *min = buffer[*d] + 1;
            } while (*min > '9');
        }
        for (i = *min; i <= '9'; i++) {
            buffer[*d] = i;
            *d += 1;
            *min = i;
            break;
        }
    }
}

int checker(const char * x, void * data)
{
    /*static int checks = 0;*/
    int p = check(x, (mpz_t *) data);
    printf("Checker for %s is %d\n", x, p);
    /*checks++;*/
    /*return checks >= 1000;*/
    return 0;
}

void dump_state(void)
{
    mpz_clear(acc);
    if (d < 0) {
        printf("Done!\n");
    } else {
        printf("State: %d \"%s\" %d %c\n", digits, buffer, d, min);
    }
}

int main(int argc, char * argv[])
{
    if (argc < 2) {
        fprintf(stderr,
                "Usage: %s <digits> [<seed>] [<d>] [<min>]\n",
                argv[0]);
        return 1;
    }
    /* Initialize state, or accept defaults. */
    if (argc >= 2) {
        digits = atoi(argv[1]);
        if (digits < 0) {
            fprintf(stderr, "Invalid number of digits: %d\n", digits);
            return 1;
        }
    }
    if (argc >= 3) {
        int i = 0;
        int len = 0;
        strncpy(buffer, argv[2], BUFFER_SZ);
        len = strlen(buffer);
        for (i = 0; i < len; i++) {
            if (buffer[i] < '0' || buffer[i] > '9') {
                fprintf(stderr,
                        "Invalid seed does not contain only digits: %s\n",
                        buffer);
                return 1;
            }
        }
    } else {
        buffer[0] = '\0';
    }
    if (argc >= 4) {
        d = atoi(argv[3]);
        if (d < 0 || d > digits) {
            fprintf(stderr, "Invalid digit index d: %d\n", d);
            return 1;
        }
    }
    if (argc >= 5) {
        min = '0' + atoi(argv[4]);
        if (min < '0' || min > '9') {
            fprintf(stderr, "Invalid minimum digit: %c\n", min);
            return 1;
        }
    }
    printf("Initialized state: digits = %d, buffer = \"%s\", d = %d, min = %c\n",
            digits, buffer, d, min);
    atexit(dump_state);
    /* Run the generator with the initialized state. */
    mpz_init(acc);
    generate(buffer, &digits, &d, &min, checker, &acc);
    return 0;
}
