MKOCTFILE ?= mkoctfile

LIB_FLAGS=-ljsoncpp

UNAME_S := $(shell uname -s)
# Big hack: on Ubuntu (and maybe other Linuxes), jsoncpp headers get installed
# under jsoncpp/json, not just json
# This hack is JsonStuff-specific
ifeq ($(UNAME_S),Linux)
	CPPFLAGS += -I /usr/include/jsoncpp
endif

# TODO: Replace this hardcoded file list with a pattern-generated
# one based on *.cc
.PHONY: all
all: __jsonstuff_jsondecode_oct__.oct

%.oct: %.cc
	$(MKOCTFILE) --verbose $(LIB_FLAGS) $<

.PHONY: clean
clean:
	$(RM) -r *.o core octave-core *.oct *~

.PHONY: distclean
distclean: clean