#include <string.h>

#ifdef __GNUC__
#define DIRSEP "/"
#else
#define DIRSEP "\\"
#endif

#ifdef CPM
char *fname(char *name) {
    char *s;
    return (s = strchr(name, ':')) ? s + 1 : name;
}
#else
char *fname(char *name) {
    char *t;
    while ((t = strpbrk(name, DIRSEP))) {
        name = t + 1;
    }
    return name;
}
#endif
