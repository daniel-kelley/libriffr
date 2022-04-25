#
#  Makefile
#
#  Copyright (c) 2021 by Daniel Kelley
#

DEBUG ?= -g

PREFIX ?= /usr/local

# address thread undefined etc.
ifneq ($(SANITIZE),)
DEBUG += -fsanitize=$(SANITIZE)
endif

INC :=
CPPFLAGS := $(INC) -MP -MMD

WARN := -Wall
WARN += -Wextra
WARN += -Wdeclaration-after-statement
WARN += -Werror
CFLAGS := $(WARN) $(DEBUG) -fPIC

LDFLAGS := $(DEBUG) -L.
LDLIBS := -lm

RIFFRINFO_SRC := riffr-info.c
RIFFRINFO_OBJ := $(RIFFRINFO_SRC:%.c=%.o)
RIFFRINFO_DEP := $(RIFFRINFO_SRC:%.c=%.d)
OBJ := $(RIFFRINFO_OBJ)
DEP := $(RIFFRINFO_DEP)

RIFFRSMF_SRC := riffr-smf.c
RIFFRSMF_OBJ := $(RIFFRSMF_SRC:%.c=%.o)
RIFFRSMF_DEP := $(RIFFRSMF_SRC:%.c=%.d)
OBJ += $(RIFFRSMF_OBJ)
DEP += $(RIFFRSMF_DEP)

LIBMAJOR := 0
LIBMINOR := 2

LIBNAME := libriffr
LIBRARY := $(LIBNAME).a
SHLIBRARY := $(LIBNAME).so
SHLIBRARY_VER := $(LIBNAME)-$(LIBMAJOR).$(LIBMINOR).so

RIFFRLIB_SRC := riffr_open.c
RIFFRLIB_SRC += riffr_valid.c
RIFFRLIB_SRC += riffr_open_internal.c
RIFFRLIB_SRC += riffr_open_smf.c
RIFFRLIB_SRC += riffr_close.c
RIFFRLIB_SRC += riffr_read_chunk_header.c
RIFFRLIB_SRC += riffr_read_chunk_body.c
RIFFRLIB_SRC += riffr_get_chunk_type.c
RIFFRLIB_SRC += riffr_read_data.c
RIFFRLIB_SRC += riffr_filename.c


RIFFRLIB_LDLIBS :=
RIFFRLIB_OBJ := $(RIFFRLIB_SRC:%.c=%.o)
RIFFRLIB_DEP := $(RIFFRLIB_SRC:%.c=%.d)
OBJ += $(RIFFRLIB_OBJ)
DEP += $(RIFFRLIB_DEP)

PROG := riffr-info
PROG += riffr-smf

.PHONY: all install uninstall clean

all: $(PROG) $(LIBRARY) $(SHLIBRARY)

riffr-info: $(RIFFRINFO_SRC) $(SHLIBRARY)

riffr-smf: $(RIFFRSMF_SRC) $(SHLIBRARY)

$(LIBRARY): $(RIFFRLIB_OBJ)
	$(AR) cr $@ $^

$(SHLIBRARY): $(SHLIBRARY_VER)
	ln -sf $< $@

$(SHLIBRARY_VER): $(RIFFRLIB_OBJ)
	$(CC) -shared -Wl,-soname,$@ -o $@ \
		$(LDFLAGS) $(RIFFRLIB_LDLIBS) $(RIFFRLIB_OBJ)

install: $(PROG) $(SHLIBRARY) $(LIBRARY)
	install -p -m 755 riffr.h $(PREFIX)/include
	install -p -m 755 $(PROG) $(PREFIX)/bin
	install -p -m 644 $(SHLIBRARY_VER) $(LIBRARY) $(PREFIX)/lib
	ln -sf -r $(PREFIX)/lib/$(SHLIBRARY_VER) $(PREFIX)/lib/$(SHLIBRARY)

uninstall:
	-rm -f $(PREFIX)/bin/$(PROG) $(PREFIX)/include/riffr.h

clean:
	-rm -f $(PROG) $(SHLIBRARY) $(SHLIBRARY_VER) $(LIBRARY) $(OBJ) $(DEP)

-include $(DEP)
