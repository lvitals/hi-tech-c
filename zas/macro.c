#include "zas.h"

/**************************************************************************
 56 2c1b +++
 **************************************************************************/
void parseParamAndBody(register sym_t *ps)
{
    sym_t *var2;
    sym_t *var3e[30];
    uint16_t cntParam;
    int16_t var42, tok;

    cntParam = 0;
    while ((tok = yylex()) != T_EOL)
    {
        if (tok == T_COMMA)
            tok = yylex();
        if (tok != G_SYM)
            syntaxErr();
        else
        {
            var2 = yylval.ySym;
            if (var2->sFlags & S_UNDEF)
                remSym(var2);
            else
                var2 = dupSym(var2);
            var2->sFlags = S_MACROPARAM;
            if (cntParam == 30)
                fatalErr("Too many macro parameters");
            var3e[cntParam++] = var2;
        }
    }
    if (phase != 0)
    {
        if (ps->mArgs)
        {
            for (var42 = 0; var42 < ps->mArgCnt;)
                free(ps->mArgs[var42++]);
            free(ps->mArgs);
        }
        if (cntParam != ps->mArgCnt)
            error("Phase error in macro args");
    }
    if (cntParam)
    {
        ps->mArgs = xalloc(cntParam * sizeof(sym_t));
    }
    else
        ps->mArgs = 0;
    ps->mArgCnt = cntParam;
    while (cntParam--)
        ps->mArgs[cntParam] = var3e[cntParam];
    skipLine();
    ps->mText = parseMacroBody();
}
/**************************************************************************
57 2dc5
**************************************************************************/
void parseMacroCall(register sym_t *ps)
{
    int var2;
    char *var4;
    if (ps->sFlags & S_MACRO)
    {
        for (var2 = 0; var2 < ps->mArgCnt; var2++)
        {
            if (!(var4 = getMacroArg()))
                var4 = "";

            ps->mArgs[var2]->mText = xalloc(strlen(var4) + 1);
            strcpy(ps->mArgs[var2]->mText, var4);
            addSym(ps->mArgs[var2]);
        }
        skipLine();
    }
    openMacro(ps);
}

/**************************************************************************
58 2e89 +++
 **************************************************************************/
void freeMacro(register sym_t *ps)
{
    int16_t var2 = 0;
    while (var2 < ps->mArgCnt)
    {
        free(ps->mArgs[var2]->mText);
        remSym(ps->mArgs[var2]);
        var2++;
    }
}
