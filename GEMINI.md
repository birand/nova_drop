# GEMINI.md - Nova Drop PRNG

## Project Overview
`nova_drop` is a lightweight, efficient pseudo-random number generator (PRNG) written in C. It is designed for applications requiring a balance between simplicity, performance, and randomness quality. It features an increased state size (currently set to 5) and employs complex bitwise operations and state interactions for thorough mixing.

### Main Technologies
- **Language:** C (C99/C11 compatible, uses `stdint.h`)
- **Core Components:** 
    - `nova_init`: Initializes the PRNG state using a 32-bit seed.
    - `nova_seed_string`: Initializes the state by hashing a string (FNV-1a).
    - `nova_auto_seed`: Automatically initializes the state using system entropy (`/dev/urandom` or system clock).
    - `nova_drop`: Generates a pseudo-random 32-bit integer and updates the state.
- **State Serialization:**
    - `nova_serialize(state, buffer)`: Exports the internal state into a `uint32_t` array of size `NOVA_DROP_STATE_SIZE`.
    - `nova_deserialize(state, buffer)`: Imports the internal state from a provided buffer.
- **Utility API:**
    - `nova_range(state, min, max)`: Generates a 32-bit integer within the specified range (inclusive).
    - `nova_float(state)`: Returns a floating-point value between `[0.0, 1.0)`.
    - `nova_bool(state)`: Returns a boolean-like integer (0 or 1).

## Building and Running

### Installation
The project can be integrated as a standard header/source pair (`nova_drop.h`/`nova_drop.c`). Alternatively, a **single-header version** is available in `nova_drop_single.h`:
```c
#define NOVA_DROP_IMPLEMENTATION
#include "nova_drop_single.h"
```

### Compilation
The project is a header/source pair designed to be integrated into larger C or C++ projects. To build the provided test program using the `Makefile`:
```bash
make
```

### Testing
A basic test and demonstration program is provided in `main.c`. To compile and run it using the `Makefile`:
```bash
make test
```

### Unit Testing
A comprehensive unit test suite using the **Unity** framework is included in `test_nova.c`.
```bash
make unit-test
```
This suite covers:
- State initialization consistency.
- Output reproducibility.
- Bounded integer generation (`nova_range`).
- Floating-point range validation (`nova_float`).
- Boolean logic validation (`nova_bool`).
- Auto-seeding entropy uniqueness.

### Statistical Quality Validation
For professional randomness validation, use the `nova_raw` utility to stream raw binary data to stdout. This can be piped into test suites:
```bash
# Example using 'rng-tools'
make nova_raw
./nova_raw | rngtest -c 1000

# Example using 'dieharder'
./nova_raw | dieharder -g 200 -a
```

### Performance Benchmarking
A benchmark utility is provided to compare `nova_drop` with the standard library's `rand()`.
```bash
make bench
```
Typical performance results show `nova_drop` achieving **~300+ MOPS** (Millions of Operations Per Second), outperforming `stdlib.h` implementations by **~2x** while providing a larger state and custom mixing logic.

### CI/CD
The project uses **GitHub Actions** for continuous integration. Every push or pull request to the `main` branch triggers an automated build and test process to ensure code quality and stability.
To clean up build artifacts:
```bash
make clean
```

## Development Conventions

### Coding Style
- **Naming:** Functions are prefixed with `nova_` (e.g., `nova_init`, `nova_drop`). Constants use `SCREAMING_SNAKE_CASE` (e.g., `NOVA_DROP_STATE_SIZE`).
- **State Management:** The PRNG state is managed by the caller using the `NovaState` struct. Users must declare a `NovaState` variable and pass its address to the functions.
- **Portability:** Uses standard `stdint.h` types for cross-platform compatibility and includes `extern "C"` guards for C++ support.

### Best Practices
- **Security:** As noted in the `README.md`, `nova_drop` is not intended for cryptographic or security-critical applications without further validation.
- **Initialization:** Always initialize the state with `nova_init` before calling `nova_drop` to ensure deterministic behavior from a given seed.

## Project Structure
- `nova_drop.h`: Public interface, constants, and `NovaState` struct.
- `nova_drop.c`: Implementation of the PRNG logic.
- `nova_drop_single.h`: Single-header (STB-style) version of the library.
- `main.c`: Example program demonstrating usage and reproducibility.
- `test_nova.c`: Comprehensive unit test suite using Unity.
- `unity.c`, `unity.h`, `unity_internals.h`: Unity C Unit Testing Framework.
- `nova_raw.c`: Utility for streaming binary data for statistical testing.
- `nova_bench.c`: Performance benchmarking utility.
- `Makefile`: Build script for the project and test suite.
- `README.md`: General overview and usage instructions.
- `LICENSE`: Open source license terms.
- `GEMINI.md`: Development guidelines and project context.
