// ctype_data.c
#include <stddef.h> // For size_t
#include <stdint.h> // For uint8_t

// Bit definitions for _ctype_ array, copied from cgen/../share/include/ctype.h
#define _U 0x01
#define _L 0x02
#define _N 0x04
#define _S 0x08
#define _P 0x10
#define _C 0x20
#define _X 0x40

// _ctype_ array definition and functions (as before)
unsigned char _ctype_[257] = {
    0, // EOF (index 0)
    _C, _C, _C, _C, _C, _C, _C, _C, _C, _S | _C, _S | _C, _S | _C, _S | _C, _S | _C, _C, _C, // 0-15
    _C, _C, _C, _C, _C, _C, _C, _C, _C, _C, _C, _C, _C, _C, _C, _C, // 16-31
    _S, _P, _P, _P, _P, _P, _P, _P, _P, _P, _P, _P, _P, _P, _P, _P, // 32-47 (space, !, ", #, $, %, &, ', (, ), *, +, ,, -, ., /)
    _N | _X, _N | _X, _N | _X, _N | _X, _N | _X, _N | _X, _N | _X, _N | _X, _N | _X, _N | _X, _P, _P, _P, _P, _P, _P, // 48-63 (0-9, :, ;, <, =, >, ?)
    _P, _U | _X, _U | _X, _U | _X, _U | _X, _U | _X, _U | _X, _U, _U, _U, _U, _U, _U, _U, _U, _U, // 64-79 (@, A-F, G-O)
    _U, _U, _U, _U, _U, _U, _U, _U, _U, _U, _U, _P, _P, _P, _P, _P, // 80-95 (P-Z, [, \, ], ^, _)
    _P, _L | _X, _L | _X, _L | _X, _L | _X, _L | _X, _L | _X, _L, _L, _L, _L, _L, _L, _L, _L, _L, // 96-111 (`, a-f, g-o)
    _L, _L, _L, _L, _L, _L, _L, _L, _L, _L, _L, _P, _P, _P, _P, _C, // 112-127 (p-z, {, |, }, ~, DEL)
    // Extended ASCII (128-255) - assuming they are not alphanumeric, control, etc. for simplicity
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 // End of 257 elements
};

int isalnum(int c) {
    if (c < 0 || c > 255) return 0;
    return (_ctype_[c+1] & (_U | _L | _N));
}

int isalpha(int c) {
    if (c < 0 || c > 255) return 0;
    return (_ctype_[c+1] & (_U | _L));
}

int isdigit(int c) {
    if (c < 0 || c > 255) return 0;
    return (_ctype_[c+1] & _N);
}

int isspace(int c) {
    if (c < 0 || c > 255) return 0;
    return (_ctype_[c+1] & _S);
}

int isupper(int c) {
    if (c < 0 || c > 255) return 0;
    return (_ctype_[c+1] & _U);
}

int isxdigit(int c) {
    if (c < 0 || c > 255) return 0;
    return (_ctype_[c+1] & (_N | _X));
}
