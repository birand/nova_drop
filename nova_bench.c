#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <stdlib.h>
#include "nova_drop.h"

/**
 * nova_bench: A simple benchmark utility to measure PRNG performance.
 * Compares nova_drop against the standard library's rand().
 */

#define ITERATIONS 100000000 // 100 million iterations

double get_time() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec * 1e-9;
}

int main() {
    NovaState state;
    nova_auto_seed(&state);

    printf("Benchmarking nova_drop vs stdlib rand() (%d iterations)...\n", ITERATIONS);

    // Benchmark nova_drop
    double start = get_time();
    uint32_t sink = 0;
    for (int i = 0; i < ITERATIONS; i++) {
        sink ^= nova_drop(&state);
    }
    double end = get_time();
    double nova_time = end - start;
    printf("nova_drop: %.4f seconds (%.2f MOPS) [sink: %u]\n", 
           nova_time, (ITERATIONS / nova_time) / 1e6, sink);

    // Benchmark stdlib rand()
    srand(time(NULL));
    start = get_time();
    sink = 0;
    for (int i = 0; i < ITERATIONS; i++) {
        sink ^= rand();
    }
    end = get_time();
    double rand_time = end - start;
    printf("rand():     %.4f seconds (%.2f MOPS) [sink: %u]\n", 
           rand_time, (ITERATIONS / rand_time) / 1e6, sink);

    printf("\nSpeed comparison: nova_drop is %.2fx %s than rand()\n",
           nova_time < rand_time ? rand_time / nova_time : nova_time / rand_time,
           nova_time < rand_time ? "FASTER" : "SLOWER");

    return 0;
}
