# GEMINI.md - Nova Drop PRNG

## Project Overview
`nova_drop` is a lightweight, efficient pseudo-random number generator (PRNG) written in C. It is designed for applications requiring a balance between simplicity, performance, and randomness quality. It features an increased state size (currently set to 5) and employs complex bitwise operations and state interactions for thorough mixing.

### Main Technologies
- **Language:** C (C99/C11 compatible, uses `stdint.h`)
- **Core Components:** 
    - `nova_init`: Initializes the PRNG state using a 32-bit seed.
    - `nova_drop`: Generates a pseudo-random 32-bit integer and updates the state.
- **Utility API:**
    - `nova_range(state, min, max)`: Generates a 32-bit integer within the specified range (inclusive).
    - `nova_float(state)`: Returns a floating-point value between `[0.0, 1.0)`.
    - `nova_bool(state)`: Returns a boolean-like integer (0 or 1).

## Building and Running

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

### Statistical Quality Validation
For professional randomness validation, use the `nova_raw` utility to stream raw binary data to stdout. This can be piped into test suites:
```bash
# Example using 'rng-tools'
make nova_raw
./nova_raw | rngtest -c 1000

# Example using 'dieharder'
./nova_raw | dieharder -g 200 -a
```
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
- `main.c`: Example program demonstrating usage and reproducibility.
- `nova_raw.c`: Utility for streaming binary data for statistical testing.
- `Makefile`: Build script for the project and test suite.
- `README.md`: General overview and usage instructions.
- `LICENSE`: Open source license terms.
- `GEMINI.md`: Development guidelines and project context.
