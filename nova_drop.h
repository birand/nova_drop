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
uint32_t nova_drop(NovaState *state);

#ifdef __cplusplus
}
#endif

#endif // NOVA_DROP_H
