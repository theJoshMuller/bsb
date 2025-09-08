OBJS = src/bsb_main.o \
       src/bsb_match.o \
       src/bsb_ref.o \
       src/bsb_render.o \
       src/intset.o \
       src/strutil.o \
       src/bsb_data.o
CFLAGS += -Wall -Isrc/
LDLIBS += -lreadline

prefix = /usr/local
exec_prefix = $(prefix)
bindir = $(exec_prefix)/bin

bsb: $(OBJS)
	$(CC) -o $@ $(LDFLAGS) $(OBJS) $(LDLIBS)

src/bsb_main.o: src/bsb_main.c src/bsb_config.h src/bsb_data.h src/bsb_match.h src/bsb_ref.h src/bsb_render.h src/strutil.h

src/bsb_match.o: src/bsb_match.h src/bsb_match.c src/bsb_config.h src/bsb_data.h src/bsb_ref.h

src/bsb_ref.o: src/bsb_ref.h src/bsb_ref.c src/intset.h src/bsb_data.h

src/bsb_render.o: src/bsb_render.h src/bsb_render.c src/bsb_config.h src/bsb_data.h src/bsb_match.h src/bsb_ref.h

src/intset.o: src/intset.h src/intset.c

src/strutil.o: src/strutil.h src/strutil.c

src/bsb_data.o: src/bsb_data.h src/bsb_data.c

src/bsb_data.c: data/bsb.tsv data/generate.awk src/bsb_data.h
	awk -f data/generate.awk $< > $@

.PHONY: clean install uninstall
clean:
	rm -rf $(OBJS) bsb

install:
	install bsb $(bindir)/bsb

uninstall:
	rm -rf $(bindir)/bsb