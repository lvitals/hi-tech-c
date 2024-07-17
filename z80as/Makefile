TARGET = z80as.com
ZAS = ../zas/zas
LINK = ../link/link

ZAS_FLAGS = -j
LINK_FLAGS = -ptext=100H,data -c100h 

AS_FILES = start.as atof.as z80as1.as z80as2.as z80as3.as z80as4.as z80as5.as alloc.as eval.as genobj.as listing.as macros.as symbols.as cpmio.as
OBJ_FILES = $(AS_FILES:.as=.obj)

all: $(TARGET)

%.obj: %.as
	$(ZAS) $(ZAS_FLAGS) $<

z80as.com: $(OBJ_FILES)
	$(LINK) $(LINK_FLAGS) -o$@ $^

clean:
	rm -f *.obj $(TARGET)

.PHONY: all clean
