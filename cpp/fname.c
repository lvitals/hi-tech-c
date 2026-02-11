#ifdef CPM
#define DIRSEP ":"
#else
#define DIRSEP "/"
#endif

#include <string.h>

char *fname(char *name) {
    char *t;
#ifdef z80
    char *s = name;
    char *p;
    while (*s != '\0') {
        for (p = DIRSEP; *p != '\0'; p++) {
            if (*s == *p) {
                t = s; /* Found a separator */
                name = t + 1; /* Advance name past the separator */
                s = name; /* Continue search from new 'name' position */
                goto next_char; /* Break out of inner loop and continue outer loop */
            }
        }
        s++;
    next_char:;
    }
    t = NULL; /* No separator found */
#else
    while ((t = strpbrk(name, DIRSEP)))
        name = t + 1;
#endif
    return name;
}
