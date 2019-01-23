MKOCTFILE ?= mkoctfile

MKOCT_LDFLAGS=-ljsoncpp

# Big hack: on Ubuntu (and maybe other Linuxes), jsoncpp headers get installed
# under jsoncpp/json, not just json
# This hack is JsonStuff-specific
MKOCT_CPPFLAGS = 
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	MKOCT_CPPFLAGS += -I/usr/include/jsoncpp
endif

# TODO: Replace this hardcoded file list with a pattern-generated
# one based on *.cc
.PHONY: all
all: __jsonstuff_jsondecode_oct__.oct

%.oct: %.cc
	echo MKOCT_CPPFLAGS: $(MKOCT_CPPFLAGS)
	echo MKOCT_LDFLAGS: $(MKOCT_LDFLAGS)
	$(MKOCTFILE) --verbose $(MKOCT_CPPFLAGS) $(MKOCT_LDFLAGS) $<

.PHONY: clean
clean:
	$(RM) -r *.o core octave-core *.oct *~

.PHONY: distclean
distclean: clean