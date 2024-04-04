// nova_drop.h

#ifndef NOVA_DROP_H
#define NOVA_DROP_H

#include <stdint.h>

#define NOVA_DROP_STATE_SIZE 5

void nova_init(uint32_t *state, uint32_t seed);
uint32_t nova_drop(uint32_t *state);

#endif // NOVA_DROP_H

