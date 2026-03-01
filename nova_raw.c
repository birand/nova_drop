#include <stdio.h>
#include <stdint.h>
#include <time.h>
#ifdef _WIN32
#include <fcntl.h>
#include <io.h>
#endif
#include "nova_drop.h"

/**
 * nova_raw: A utility to stream raw binary data from the nova_drop PRNG to stdout.
 * This is useful for piping into statistical test suites like:
 *   ./nova_raw | dieharder -g 200 -a
 *   ./nova_raw | rngtest -c 1000
 */

int main(int argc, char *argv[]) {
    NovaState state;
    uint32_t seed;

    if (argc > 1) {
        seed = (uint32_t)atoll(argv[1]);
    } else {
        seed = (uint32_t)time(NULL);
    }

    nova_init(&state, seed);

    // Ensure stdout is in binary mode (critical for Windows)
#ifdef _WIN32
    _setmode(_fileno(stdout), _O_BINARY);
#endif

    while (1) {
        uint32_t val = nova_drop(&state);
        if (fwrite(&val, sizeof(uint32_t), 1, stdout) != 1) {
            break;
        }
    }

    return 0;
}
