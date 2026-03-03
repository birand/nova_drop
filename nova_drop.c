// nova_drop.c

#include <stdio.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h>
#include <bcrypt.h>
#pragma comment(lib, "bcrypt.lib")
#endif

#include "nova_drop.h"

void nova_init(NovaState *state, uint32_t seed) {
    uint32_t i;
    state->state[0] = seed;
    state->index = 0;
    for (i = 1; i < NOVA_DROP_STATE_SIZE; ++i) {
        state->state[i] = 1812433253 * (state->state[i - 1] ^ (state->state[i - 1] >> 30)) + i;
    }
}

void nova_seed_string(NovaState *state, const char *key) {
    uint32_t hash = 2166136261u;
    if (key) {
        while (*key) {
            hash ^= (uint32_t)(*key++);
            hash *= 16777619u;
        }
    }
    nova_init(state, hash);
}

void nova_auto_seed(NovaState *state) {
    uint32_t seed;
    int success = 0;

#ifdef _WIN32
    if (BCryptGenRandom(NULL, (PUCHAR)&seed, sizeof(seed), BCRYPT_USE_SYSTEM_PREFERRED_RNG) == 0) {
        success = 1;
    }
#else
    FILE *urandom = fopen("/dev/urandom", "rb");
    if (urandom) {
        if (fread(&seed, sizeof(uint32_t), 1, urandom) == 1) {
            success = 1;
        }
        fclose(urandom);
    }
#endif

    if (!success) {
        seed = (uint32_t)(time(NULL) ^ clock());
    }
    nova_init(state, seed);
}

uint32_t nova_drop(NovaState *state) {
    uint32_t t, xorshifted, rot;
    
    // Rolling update: only update one element per call
    uint32_t i = state->index;
    uint32_t next = (i + 1) & (NOVA_DROP_STATE_SIZE - 1); // Fast bitwise wrapping
    
    t = state->state[next];
    xorshifted = ((t ^ (t >> 2)) ^ (t ^ (t >> 13))) ^ (t ^ (t >> 22));
    rot = t >> (t >> 28);
    state->state[i] = xorshifted ^ (rot | (rot << 4));
    
    state->index = next;
    
    // Return a combined XOR of the state for high randomness depth
    return state->state[0] ^ state->state[1] ^ state->state[2] ^ state->state[3];
}

void nova_serialize(const NovaState *state, uint32_t *buffer) {
    uint32_t i;
    for (i = 0; i < NOVA_DROP_STATE_SIZE; i++) {
        buffer[i] = state->state[i];
    }
    buffer[NOVA_DROP_STATE_SIZE] = state->index;
}

void nova_deserialize(NovaState *state, const uint32_t *buffer) {
    uint32_t i;
    for (i = 0; i < NOVA_DROP_STATE_SIZE; i++) {
        state->state[i] = buffer[i];
    }
    state->index = buffer[NOVA_DROP_STATE_SIZE];
}

void nova_jump(NovaState *state) {
    // For non-linear generators, a jump is often implemented as a fixed
    // large number of iterations to statistically separate streams.
    for (int i = 0; i < 256; i++) {
        nova_drop(state);
    }
}

uint32_t nova_range(NovaState *state, uint32_t min, uint32_t max) {
    if (min >= max) return min;
    return min + (nova_drop(state) % (max - min + 1));
}

float nova_float(NovaState *state) {
    return (float)nova_drop(state) / (float)UINT32_MAX;
}

int nova_bool(NovaState *state) {
    return (int)(nova_drop(state) & 1);
}
