#include "zas.h"

tmpLabel_t *highLabel;           /* a75a */
tmpLabel_t tmpLabelTab[MAX_TMP]; /* a756 */
tmpLabel_t *tmpLabelPtr;         /* af90 */
tmpLabel_t *lowLabel;            /* af92 */

/**************************************************************************
 112	sub_53e5h +++
 **************************************************************************/
void initLocalLabels()
{
#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Warray-bounds"
#endif
    lowLabel = tmpLabelTab - 1; /* deliberately before start, but always incremented before use */
#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

    if (phase != 2)
        tmpLabelPtr = lowLabel;
    if (phase == 0)
        highLabel = lowLabel;
}

/**************************************************************************
 113	sub_5404h +++
 **************************************************************************/
void defTmpLabel(int16_t tLabel)
{
    if (phase != 2)
    {
        if (++tmpLabelPtr == &tmpLabelTab[MAX_TMP])
            fatalErr("Too many temporary labels");
        tmpLabelPtr->tLabel = tLabel;
        tmpLabelPtr->tLoc = curPsect->pCurLoc;
        tmpLabelPtr->tPsect = curPsect;
        if (phase == 0)
            highLabel = tmpLabelPtr;
    }

    lowLabel++;
}

/**************************************************************************
 114 546a +++
 **************************************************************************/
rval_t *findLocalLabel(int16_t nLabel, int tok)
{
    register tmpLabel_t *iy = lowLabel;
    if (tok == G_FWD)
    {
        do
        {
            if (++iy > highLabel)
                goto undef;
        } while (iy->tLabel != nLabel);
    }
    else
    {
        while (iy->tLabel != nLabel)
            if (--iy < tmpLabelTab)
                goto undef;
    }
    retProp.val = iy->tLoc;
    retProp.pSym = iy->tPsect;
    return &retProp;
undef:
    error("Undefined temporary label");
    retProp.pSym = absPsect;
    retProp.val = 0;
    return &retProp;
}
