/**
 * @file nova_drop.h
 * @brief High-performance pseudo-random number generator (PRNG) for C/C++.
 * 
 * nova_drop provides a lightweight, thread-safe, and efficient way to generate
 * pseudo-random numbers with a balance of speed and randomness quality.
 */

#ifndef NOVA_DROP_H
#define NOVA_DROP_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief The internal state size of the nova_drop PRNG (power of 2).
 */
#define NOVA_DROP_STATE_SIZE 4

/**
 * @struct NovaState
 * @brief Holds the internal state of the PRNG.
 * 
 * This struct must be initialized using nova_init, nova_auto_seed, 
 * or nova_seed_string before being used with other functions.
 */
typedef struct {
    uint32_t state[NOVA_DROP_STATE_SIZE]; /**< The internal 128-bit state array. */
    uint32_t index;                       /**< The current rolling index for updates. */
} NovaState;

/**
 * @brief Initializes the PRNG with a specific 32-bit integer seed.
 * 
 * @param state Pointer to the NovaState struct to initialize.
 * @param seed A 32-bit integer seed for deterministic sequences.
 */
void nova_init(NovaState *state, uint32_t seed);

/**
 * @brief Initializes the PRNG by hashing a string key.
 * 
 * Uses the FNV-1a hashing algorithm to convert the string into a seed.
 * 
 * @param state Pointer to the NovaState struct to initialize.
 * @param key The string used as the seed phrase.
 */
void nova_seed_string(NovaState *state, const char *key);

/**
 * @brief Automatically seeds the PRNG using system entropy.
 * 
 * Uses /dev/urandom on Unix-like systems and BCryptGenRandom on Windows.
 * Falls back to system clock (time/clock) if entropy sources are unavailable.
 * 
 * @param state Pointer to the NovaState struct to initialize.
 */
void nova_auto_seed(NovaState *state);

/**
 * @brief Generates a pseudo-random 32-bit unsigned integer.
 * 
 * This is the core generator function that updates the internal state.
 * 
 * @param state Pointer to the NovaState struct.
 * @return A pseudo-random uint32_t value.
 */
uint32_t nova_drop(NovaState *state);

/**
 * @brief Exports the internal state into a portable buffer.
 * 
 * Useful for saving the PRNG state to disk or memory.
 * 
 * @param state Pointer to the NovaState struct.
 * @param buffer A uint32_t array of size NOVA_DROP_STATE_SIZE.
 */
void nova_serialize(const NovaState *state, uint32_t *buffer);

/**
 * @brief Imports the internal state from a portable buffer.
 * 
 * Useful for resuming a previously saved PRNG sequence.
 * 
 * @param state Pointer to the NovaState struct.
 * @param buffer A uint32_t array of size NOVA_DROP_STATE_SIZE.
 */
void nova_deserialize(NovaState *state, const uint32_t *buffer);

/**
 * @brief Advances the PRNG state by a massive number of steps (2^64 equivalent).
 * 
 * Useful for parallel processing to ensure different threads have non-overlapping
 * sequences. In this non-linear implementation, it performs a long-step mixing.
 * 
 * @param state Pointer to the NovaState struct.
 */
void nova_jump(NovaState *state);

/**
 * @brief Generates a random integer within a specified range [min, max].
 * 
 * @param state Pointer to the NovaState struct.
 * @param min The lower bound (inclusive).
 * @param max The upper bound (inclusive).
 * @return A random uint32_t within the range.
 */
uint32_t nova_range(NovaState *state, uint32_t min, uint32_t max);

/**
 * @brief Generates a random float in the range [0.0, 1.0).
 * 
 * @param state Pointer to the NovaState struct.
 * @return A random float value.
 */
float nova_float(NovaState *state);

/**
 * @brief Generates a random boolean-like integer (0 or 1).
 * 
 * @param state Pointer to the NovaState struct.
 * @return 0 or 1.
 */
int nova_bool(NovaState *state);

#ifdef __cplusplus
}
#endif

#endif // NOVA_DROP_H
