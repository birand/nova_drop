// nova_drop.c

#include "nova_drop.h"

void nova_init(uint32_t *state, uint32_t seed) {
    uint32_t i;
    state[0] = seed;
    for (i = 1; i < NOVA_DROP_STATE_SIZE; ++i) {
        state[i] = 1812433253 * (state[i - 1] ^ (state[i - 1] >> 30)) + i;
    }
}

uint32_t nova_drop(uint32_t *state) {
    uint32_t i, t, xorshifted, rot;
    for (i = 0; i < NOVA_DROP_STATE_SIZE; ++i) {
        t = state[(i + 1) % NOVA_DROP_STATE_SIZE];
        xorshifted = ((t ^ (t >> 2)) ^ (t ^ (t >> 13))) ^ (t ^ (t >> 22));
        rot = t >> (t >> 28);
        state[i] = xorshifted ^ (rot | (rot << 4));
    }

    uint32_t result = 0;
    for (i = 0; i < NOVA_DROP_STATE_SIZE; ++i) {
        result ^= state[i];
    }
    return result;
}

