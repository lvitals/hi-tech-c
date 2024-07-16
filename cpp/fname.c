#ifdef CPM
#define DIRSEP ":"
#else
#define DIRSEP "/"
#endif

#include <string.h>

char *fname(char *name) {
    char *t;
    while ((t = strpbrk(name, DIRSEP)))
        name = t + 1;
    return name;
}
