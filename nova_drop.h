#ifndef NOVA_DROP_H
#define NOVA_DROP_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define NOVA_DROP_STATE_SIZE 5

typedef struct {
    uint32_t state[NOVA_DROP_STATE_SIZE];
} NovaState;

void nova_init(NovaState *state, uint32_t seed);
void nova_auto_seed(NovaState *state);
uint32_t nova_drop(NovaState *state);

// State Serialization
void nova_serialize(const NovaState *state, uint32_t *buffer);
void nova_deserialize(NovaState *state, const uint32_t *buffer);

// Utility functions
uint32_t nova_range(NovaState *state, uint32_t min, uint32_t max);
float nova_float(NovaState *state);
int nova_bool(NovaState *state);

#ifdef __cplusplus
}
#endif

#endif // NOVA_DROP_H
