/**
 * @file nova_drop_single.h
 * @brief Single-header version of the nova_drop PRNG.
 * 
 * To use the implementation, define NOVA_DROP_IMPLEMENTATION in ONE C/C++ file:
 * #define NOVA_DROP_IMPLEMENTATION
 * #include "nova_drop_single.h"
 */

#ifndef NOVA_DROP_SINGLE_H
#define NOVA_DROP_SINGLE_H

#include <stdint.h>
#include <stdio.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h>
#include <bcrypt.h>
#pragma comment(lib, "bcrypt.lib")
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define NOVA_DROP_STATE_SIZE 4

typedef struct {
    uint32_t state[NOVA_DROP_STATE_SIZE];
    uint32_t index;
} NovaState;

void nova_init(NovaState *state, uint32_t seed);
void nova_seed_string(NovaState *state, const char *key);
void nova_auto_seed(NovaState *state);
uint32_t nova_drop(NovaState *state);
void nova_serialize(const NovaState *state, uint32_t *buffer);
void nova_deserialize(NovaState *state, const uint32_t *buffer);
void nova_jump(NovaState *state);
uint32_t nova_range(NovaState *state, uint32_t min, uint32_t max);
float nova_float(NovaState *state);
int nova_bool(NovaState *state);

#ifdef __cplusplus
}
#endif

#ifdef NOVA_DROP_IMPLEMENTATION

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
    uint32_t i = state->index;
    uint32_t next = (i + 1) & (NOVA_DROP_STATE_SIZE - 1);
    
    t = state->state[next];
    xorshifted = ((t ^ (t >> 2)) ^ (t ^ (t >> 13))) ^ (t ^ (t >> 22));
    rot = t >> (t >> 28);
    state->state[i] = xorshifted ^ (rot | (rot << 4));
    
    state->index = next;
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
    for (int i = 0; i < 256; i++) {
        nova_drop(state);
    }
}

uint32_t nova_range(NovaState *state, uint32_t min, uint32_t max) {
    if (min >= max) return min;
    return min + (nova_drop(state) % (max - min + 1));
}

float nova_float(NovaState *state) {
    return (float)nova_drop(state) / (float)0xFFFFFFFFu;
}

int nova_bool(NovaState *state) {
    return (int)(nova_drop(state) & 1);
}

#endif // NOVA_DROP_IMPLEMENTATION
#endif // NOVA_DROP_SINGLE_H
