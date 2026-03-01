// nova_drop.c

#include "nova_drop.h"

void nova_init(NovaState *state, uint32_t seed) {
    uint32_t i;
    state->state[0] = seed;
    for (i = 1; i < NOVA_DROP_STATE_SIZE; ++i) {
        state->state[i] = 1812433253 * (state->state[i - 1] ^ (state->state[i - 1] >> 30)) + i;
    }
}

uint32_t nova_drop(NovaState *state) {
    uint32_t i, t, xorshifted, rot;
    for (i = 0; i < NOVA_DROP_STATE_SIZE; ++i) {
        t = state->state[(i + 1) % NOVA_DROP_STATE_SIZE];
        xorshifted = ((t ^ (t >> 2)) ^ (t ^ (t >> 13))) ^ (t ^ (t >> 22));
        rot = t >> (t >> 28);
        state->state[i] = xorshifted ^ (rot | (rot << 4));
    }

    uint32_t result = 0;
    for (i = 0; i < NOVA_DROP_STATE_SIZE; ++i) {
        result ^= state->state[i];
    }
    return result;
}

