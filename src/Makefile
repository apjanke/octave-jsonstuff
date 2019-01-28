MKOCTFILE ?= mkoctfile

MKOCT_LDFLAGS=-ljsoncpp

.PHONY: all clean distclean

# JsonCpp (and therefore our code) need c++11
export CXXFLAGS := $(CXXFLAGS) -std=gnu++11

# Hack: assume all .cc files are oct-file sources; this works because the JsonCpp
# code uses the .cpp extension
OCT_SOURCES = $(wildcard *.cc)
OCT_FILES = $(patsubst %.cc,%.oct,$(OCT_SOURCES))

# Hack: just directly compile the JsonCpp library into every oct-file
JSONCPP_SOURCES = jsoncpp.cpp

all: $(OCT_FILES)

# Argument order matters here! The .oct file is named after the first source fil
# passed to mkoctfile.
%.oct: %.cc
	echo MKOCT_CPPFLAGS: $(MKOCT_CPPFLAGS)
	echo MKOCT_LDFLAGS: $(MKOCT_LDFLAGS)
	$(MKOCTFILE) --verbose $(MKOCT_CPPFLAGS) $(MKOCT_LDFLAGS) $< $(JSONCPP_SOURCES)

clean:
	$(RM) -r *.o *.a core octave-core *.oct *~

distclean: clean
