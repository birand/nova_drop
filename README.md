# Nova Drop Pseudo-Random Number Generator

## Overview

`nova_drop` is a lightweight, easy-to-use pseudo-random number generator (PRNG) designed for applications requiring random number generation with a good balance between simplicity, efficiency, and randomness quality. It features an increased state size and complex state interactions to enhance the randomness of its output.

## Features

- Increased state size for longer periods and improved randomness.
- Complex bitwise operations ensure thorough mixing of state.
- Easy to integrate into any C or C++ project.
- Designed with simplicity and performance in mind.

## Getting Started

### Prerequisites

- A C compiler (e.g., GCC, Clang)
- Basic knowledge of C programming

### Compilation

1. Save `nova_drop.c` and `nova_drop.h` in your project directory.
2. Include `nova_drop.h` in your source file: `#include "nova_drop.h"`.
3. Compile your project. For example, if using GCC and your source file is `main.c`:

```bash
gcc -o my_program main.c nova_drop.c
```
### Usage
Initialize the PRNG State: Before generating random numbers, initialize the state with a seed.
```bash
uint32_t state[NOVA_DROP_STATE_SIZE];
uint32_t seed = 123456789; // Example seed
nova_init(state, seed);
```
Generate Random Numbers: Use nova_drop to generate random numbers as needed.
```bash
uint32_t random_number = nova_drop(state);
```
## Documentation

## Functions
void nova_init(uint32_t *state, uint32_t seed): Initializes the PRNG state with a given seed.
uint32_t nova_drop(uint32_t *state): Generates a pseudo-random number based on the current state and updates the state.
## Constants
NOVA_DROP_STATE_SIZE: The size of the state array required by nova_drop. Currently set to 5.
### License

nova_drop is open source and freely available for personal and commercial use.

### Disclaimer

While nova_drop has been designed with care, it has not undergone extensive testing in cryptographic contexts and should not be used for security-critical applications without further validation.

### Contributions

Contributions, suggestions, and bug reports are welcome and can be submitted via the project's GitHub repository.

### Contact

For questions or support, please open an issue on the GitHub project page.
