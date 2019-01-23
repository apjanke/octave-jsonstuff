MKOCTFILE ?= mkoctfile

MKOCT_LDFLAGS=-ljsoncpp

# Big hack: on Ubuntu (and maybe other Linuxes), jsoncpp headers get installed
# under jsoncpp/json, not just json
# This hack is JsonStuff-specific
MKOCT_CPPFLAGS =
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	MKOCT_CPPFLAGS += -I/usr/include/jsoncpp
	export CXXFLAGS := $(CXXFLAGS) -std=gnu++14
endif

OCT_SOURCES = $(wildcard *.cc)
OCT_FILES = $(patsubst %.cc,%.oct,$(OCT_SOURCES))

.PHONY: all
all: $(OCT_FILES)

%.oct: %.cc
	echo MKOCT_CPPFLAGS: $(MKOCT_CPPFLAGS)
	echo MKOCT_LDFLAGS: $(MKOCT_LDFLAGS)
	$(MKOCTFILE) --verbose $(MKOCT_CPPFLAGS) $(MKOCT_LDFLAGS) $<

.PHONY: clean
clean:
	$(RM) -r *.o core octave-core *.oct *~

.PHONY: distclean
distclean: clean
