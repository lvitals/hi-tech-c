/*
    Hitech P1.COM makes use of _Hbss to determine whether a memory location is above or below
    the data areas.
    Whilst this works for CP/M & MSDOS where the image is ordered as
    text, data, bss with heap and stack above this, it may not work for other OS and compilers
    in particular, even thought the user code is often still often in the same order, dynamic
    libraries will not fit this model and it is quite possible for stack and heap to be below the
    text area causing the simple tests in P1.COM to fail

    As only P1.COM allocated addresses are used in the memory checks, this code sets _Ldata and _Hbss
    to define the boundaries of the data area.
    It assumes read only data, data and bss are adjacent blocks and heap/stack are not interleaved
    For linux / GCC the compiler labels etext and end are used and mapped as follows
    etext ->  _Ldata
    end   ->  _Hbss
    For windows only the .rdata and .data segments are considered, others are assumed not to
    contain P1.COM specific data.
    start .rdata ->_Ldata
    end   .data + sizeof(.data) - 1  ->_Hbss
    Note this file is not needed for compilation on CP/M and will fail to build due to the
    #pragma
*/

#ifdef __GNUC__
#ifndef __APPLE__
extern char etext, end;
const char *_Ldata = &etext;
const char *_Hbss = &end;
#endif
#elif defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
/* force windows.h first */
#include <DbgHelp.h>
#pragma comment(lib, "dbghelp.lib")

const char *_Ldata;
const char *_Hbss;

void initMemAddr(void) {
    HANDLE hModule = GetModuleHandle(NULL); /* module handle */

    /* get the location of the module's IMAGE_NT_HEADERS structure */
    IMAGE_NT_HEADERS *pNtHdr = ImageNtHeader(hModule);

    /* section table immediately follows the IMAGE_NT_HEADERS */
    IMAGE_SECTION_HEADER *pSectionHdr = (IMAGE_SECTION_HEADER *)(pNtHdr + 1);

    const char *imageBase             = (const char *)hModule;

    for (int scn = 0; scn < pNtHdr->FileHeader.NumberOfSections; ++scn) {
        if (strcmp(pSectionHdr->Name, ".rdata") == 0)
            _Ldata = imageBase + pSectionHdr->VirtualAddress;
        else if (strcmp(pSectionHdr->Name, ".data") == 0)
            _Hbss = imageBase + pSectionHdr->VirtualAddress + pSectionHdr->Misc.VirtualSize - 1;
        ++pSectionHdr;
    }
}
#else
#error code behind inData missing
#endif
