# Nova Drop PRNG

[![C/C++ CI](https://github.com/birand/nova_drop/actions/workflows/c-cpp.yml/badge.svg)](https://github.com/birand/nova_drop/actions/workflows/c-cpp.yml)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

`nova_drop` is a high-performance, lightweight pseudo-random number generator (PRNG) written in C. It is designed to offer a balance between simplicity, speed (~100 MOPS), and randomness quality for simulations, games, and general-purpose applications.

## Key Features

- **Fast & Efficient:** Capable of generating ~100 million numbers per second (MOPS), highly competitive with `stdlib.h` implementations.
- **Thread-Safe by Design:** Uses a caller-owned `NovaState` struct, avoiding global state and supporting concurrent environments.
- **Robust Seeding:** Support for manual 32-bit seeds or automatic high-entropy seeding via `/dev/urandom`.
- **Expanded API:** Built-in utilities for bounded ranges, floats `[0, 1)`, and boolean flips.
- **C++ Compatible:** Includes `extern "C"` guards for seamless integration into C++ projects.
- **Comprehensive Testing:** Validated with a Unity-based unit test suite and a raw binary streaming utility for statistical analysis.

## Performance Benchmark

Running on a standard environment, `nova_drop` shows the following throughput:

| Implementation | Throughput (MOPS) | Time (100M iter) |
|----------------|-------------------|------------------|
| `nova_drop`    | **~98.5 MOPS**    | ~1.01s           |
| `stdlib rand()`| ~100.1 MOPS       | ~0.99s           |

*Benchmarks performed with `gcc -O3`. `nova_drop` achieves near-stdlib performance while maintaining a significantly larger internal state and more complex mixing.*

## Installation

Simply add `nova_drop.c` and `nova_drop.h` to your project and include the header:

```c
#include "nova_drop.h"
```

## Quick Start

### Auto-Seeding (Recommended)
```c
NovaState state;
nova_auto_seed(&state); // Uses /dev/urandom or system clock
uint32_t val = nova_drop(&state);
```

### Manual Seeding & Utilities
```c
NovaState state;
nova_init(&state, 12345);

uint32_t dice = nova_range(&state, 1, 6);  // [1, 6]
float chance = nova_float(&state);         // [0.0, 1.0)
int coin = nova_bool(&state);              // 0 or 1
```

## API Reference

- `void nova_init(NovaState *state, uint32_t seed)`: Initialize with a specific seed.
- `void nova_auto_seed(NovaState *state)`: Initialize using system entropy.
- `uint32_t nova_drop(NovaState *state)`: Generate a 32-bit random integer.
- `uint32_t nova_range(NovaState *state, uint32_t min, uint32_t max)`: Generate a number in `[min, max]`.
- `float nova_float(NovaState *state)`: Generate a float in `[0.0, 1.0)`.
- `int nova_bool(NovaState *state)`: Generate a 0 or 1.

## Development & Testing

The project includes a multi-target `Makefile`:

- `make test`: Build and run the basic demonstration.
- `make unit-test`: Run the comprehensive Unity-based test suite.
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
