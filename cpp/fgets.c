#include <stdio.h>

char *fgets(char *s, int n, FILE *stream)
{
    int c;
    char *cs = s;
    
    if (n <= 0)
        return 0;
    
    while (--n > 0)
    {
        c = fgetc(stream);
        if (c == EOF)
        {
            *cs = '\0';
            if (cs == s)
                return 0;
            return s;
        }
        
        *cs++ = (char)c;
        
        if (c == '\n')
            break;
    }
    
    *cs = '\0';
    return s;
}