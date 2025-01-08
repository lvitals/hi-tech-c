#ifndef __GNUC__
extern char etext, end;
const char *_Ldata = &etext;
const char *_Hbss = &end;
#else
// #error Unix specific implementation is required
#endif
