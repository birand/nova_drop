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

    // Demonstrate reproducibility with the same seed
    NovaState state2;
    nova_init(&state2, seed);
    printf("\nRe-initializing with same seed (42). First number: %u (Should match first above)\n", nova_drop(&state2));

    return 0;
}
