# Nova Drop PRNG

[![C/C++ CI](https://github.com/birand/nova_drop/actions/workflows/c-cpp.yml/badge.svg)](https://github.com/birand/nova_drop/actions/workflows/c-cpp.yml)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

`nova_drop` is a ultra-high-performance, lightweight pseudo-random number generator (PRNG) written in C. It is designed to offer a balance between simplicity, extreme speed (~300+ MOPS), and randomness quality for simulations, games, and general-purpose applications.

## Key Features

- **Extreme Speed:** Capable of generating **~300+ million numbers per second (MOPS)**, typically **2.3x faster** than `stdlib.h` implementations.
- **Thread-Safe by Design:** Uses a caller-owned `NovaState` struct, avoiding global state and supporting concurrent environments.
- **Robust Seeding:** Support for manual 32-bit seeds, string-based phrases (FNV-1a), or automatic high-entropy seeding via `/dev/urandom` (Unix) or `BCryptGenRandom` (Windows).
- **Expanded API:** Built-in utilities for bounded ranges, floats `[0, 1)`, boolean flips, and sequence "jumping" for parallel processing.
- **State Serialization:** Easily save and load the PRNG state for persistence or networking.
- **Single-Header Option:** Available as an STB-style single-header library (`nova_drop_single.h`) for zero-configuration integration.
- **Doxygen Documented:** Fully commented header for professional IDE tooltips and automated documentation.

## Performance Benchmark

Running on a standard environment, `nova_drop` shows the following throughput:

| Implementation | Throughput (MOPS) | Time (100M iter) |
|----------------|-------------------|------------------|
| `nova_drop`    | **~316.0 MOPS**   | **~0.32s**       |
| `stdlib rand()`| ~135.0 MOPS       | ~0.74s           |

*Benchmarks performed with `gcc -O3`. `nova_drop` significantly outperforms `stdlib.h` while maintaining a 128-bit internal state and more complex mixing.*

## Installation

The project can be integrated as a standard header/source pair. Alternatively, use the **single-header version** (`nova_drop_single.h`) for zero-configuration integration:

```c
#define NOVA_DROP_IMPLEMENTATION
#include "nova_drop_single.h"
```

## Quick Start

### Auto-Seeding & Strings
```c
NovaState state;
nova_auto_seed(&state);                 // Uses OS entropy (Unix/Windows)
nova_seed_string(&state, "my_phrase");  // Seed with a string
```

### Utilities & Parallelism
```c
NovaState state;
nova_init(&state, 12345);

uint32_t dice = nova_range(&state, 1, 6);  // [1, 6]
float chance = nova_float(&state);         // [0.0, 1.0)
int coin = nova_bool(&state);              // 0 or 1

// Jump the sequence forward for a parallel thread
nova_jump(&state); 
```

## API Reference

- `void nova_init(NovaState *state, uint32_t seed)`: Initialize with a specific seed.
- `void nova_seed_string(NovaState *state, const char *key)`: Initialize by hashing a string phrase.
- `void nova_auto_seed(NovaState *state)`: Initialize using system entropy (Unix/Windows).
- `uint32_t nova_drop(NovaState *state)`: Generate a 32-bit random integer.
- `uint32_t nova_range(NovaState *state, uint32_t min, uint32_t max)`: Generate a number in `[min, max]`.
- `float nova_float(NovaState *state)`: Generate a float in `[0.0, 1.0)`.
- `int nova_bool(NovaState *state)`: Generate a 0 or 1.
- `void nova_serialize(const NovaState *state, uint32_t *buffer)`: Export state to a buffer.
- `void nova_deserialize(NovaState *state, const uint32_t *buffer)`: Import state from a buffer.
- `void nova_jump(NovaState *state)`: Advance state by $2^{64}$ equivalent steps for parallel streams.

## Development & Testing

The project includes a multi-target `Makefile`:

- `make test`: Build and run the basic demonstration.
- `make unit-test`: Run the comprehensive Unity-based test suite (10+ tests).
- `make bench`: Run the performance benchmark utility.
- `make nova_raw`: Build the binary streaming utility (pipe to `dieharder` or `rngtest`).

```bash
# Example: Statistical Validation
make nova_raw
./nova_raw | rngtest -c 1000
```

## License & Disclaimer

Distributed under the **MIT License**. See `LICENSE` for details.

**Disclaimer:** `nova_drop` is intended for non-cryptographic purposes. It has not undergone formal security audits and should not be used for security-critical applications (e.g., encryption keys, sensitive hashing) without further validation.
