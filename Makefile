# SUBDIRS = cgen cpm cpp huff link objtohex optim p1 zas z80as
SUBDIRS = cgen cpm cpp link p1 zas z80as

all: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@

clean:
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir clean; \
	done

.PHONY: all clean $(SUBDIRS)
