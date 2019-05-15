#ifndef TYPES_REDEFINE_H
#define TYPES_REDEFINE_H

#include <stdint.h>

// Redefine TRUE and FALSE in case they are no more declared in GTK+

#undef TRUE
#undef FALSE
#define FALSE (0)
#define TRUE (!FALSE)

// sized exactly
// redifined for more comprehension in the code

typedef int8_t        int8; // char = 1 byte = 8 bits
typedef uint8_t       Uint8;
typedef int16_t       int16; // short = 2 bytes = 16 bits
typedef uint16_t      Uint16;
typedef int32_t       int32; // long = 4 bytes = 32 bits
typedef uint32_t      Uint32;
typedef int64_t       int64; // long long = 8 bytes = 64 bits
typedef uint64_t      Uint64;

typedef int8        bool;





#endif /* TYPES_REDEFINE_H */
