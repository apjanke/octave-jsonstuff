MKOCTFILE ?= mkoctfile

LIB_FLAGS=-ljsoncpp

# TODO: Replace this hardcoded file list with a pattern-generated
# one based on *.cc
.PHONY: all
all: __jsonstuff_jsondecode_oct__.oct

%.oct: %.cc
	$(MKOCTFILE) $(LIB_FLAGS) $<

.PHONY: clean
clean:
	$(RM) -r *.o core octave-core *.oct *~

.PHONY: distclean
distclean: clean