#include "p1.h"

#define _Z          0  /* 0000 0000 */
#define _U          1  /* 0000 0001 */
#define _L          2  /* 0000 0010 */
#define _D          4  /* 0000 0100 */
#define _X          8  /* 0100 0000 */
#define _S          16 /* 0001 0000 */

/* clang-format off */
static char ccClass[] = { /* 8134 */
                   _Z,                                                                      /* EOF */
                   _Z,      _Z,      _Z,      _Z,      _Z,      _Z,      _Z,      _Z,       /* 0  */
                   _S,      _S,      _S,      _Z,      _S,      _S,      _Z,      _Z,       /* 8 */
                   _Z,      _Z,      _Z,      _Z,      _Z,      _Z,      _Z,      _Z,       /* 10 */
                   _Z,      _Z,      _Z,      _Z,      _Z,      _Z,      _Z,      _Z,       /* 18 */
                   _S,      _Z,      _Z,      _Z,      _Z,      _Z,      _Z,      _Z,       /* 20 */
                   _Z,      _Z,      _Z,      _Z,      _Z,      _Z,      _Z,      _Z,       /* 28 */
                   _D | _X, _D | _X, _D | _X, _D | _X, _D | _X, _D | _X, _D | _X, _D | _X,  /* 30 */
                   _D | _X, _D | _X, _Z,      _Z,      _Z,      _Z,      _Z,      _Z,       /* 38 */
                   _Z,      _U | _X, _U | _X, _U | _X, _U | _X, _U | _X, _U | _X, _U,       /* 40 */
                   _U,      _U,      _U,      _U,      _U,      _U,      _U,      _U,       /* 48 */
                   _U,      _U,      _U,      _U,      _U,      _U,      _U,      _U,       /* 50 */
                   _U,      _U,      _U,      _Z,      _Z,      _Z,      _Z,      _U,       /* 58 */
                   _Z,      _L | _X, _L | _X, _L | _X, _L | _X, _L | _X, _L | _X, _L,       /* 60 */
                   _L,      _L,      _L,      _L,      _L,      _L,      _L,      _L,       /* 68 */
                   _L,      _L,      _L,      _L,      _L,      _L,      _L,      _L,       /* 70 */
                   _L,      _L,      _L,      _Z,      _Z,      _Z,      _Z,      _Z        /* 78 */
};
/* clang-format on */

int Isalpha(int c) { return ((ccClass + 1)[(int16_t)c] & (_U | _L)); }
int Isupper(int c) { return ((ccClass + 1)[(int16_t)c] & _U); }
int Islower(int c) { return ((ccClass + 1)[(int16_t)c] & _L); }
int Isdigit(int c) { return ((ccClass + 1)[(int16_t)c] & _D); }
int Isxdigit(int c) { return ((ccClass + 1)[(int16_t)c] & _X); }
int Isspace(int c) { return ((ccClass + 1)[(int16_t)c] & _S); }
int Isalnum(int c) { return ((ccClass + 1)[(int16_t)c] & (_U | _L | _D)); }
int Tolower(int c) { return (Isupper(c) ? c + 0x20 : c); }
