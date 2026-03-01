#include <stdio.h>
#include "nova_drop.h"

int main() {
    NovaState state;
    uint32_t seed = 42;

    printf("Initializing nova_drop with seed: %u\n", seed);
    nova_init(&state, seed);

    printf("Generating 10 random numbers:\n");
    for (int i = 0; i < 10; i++) {
        printf("%u\n", nova_drop(&state));
    }

    NovaState state2;
    nova_init(&state2, seed);
    printf("\nRe-initializing with same seed (42). First number: %u (Should match first above)\n", nova_drop(&state2));

    printf("\nTesting Utility Functions:\n");
    printf("Range [1, 10]: %u\n", nova_range(&state, 1, 10));
    printf("Float [0, 1): %f\n", nova_float(&state));
    printf("Boolean flip: %s\n", nova_bool(&state) ? "TRUE" : "FALSE");

    return 0;
}
