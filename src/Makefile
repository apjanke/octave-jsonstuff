MKOCTFILE ?= mkoctfile

.PHONY: all
all: __jsonstuff_jsondecode_oct__.oct

%.oct: %.cc
	$(MKOCTFILE) -ljsoncpp $<

.PHONY: clean
clean:
	$(RM) -r *.o core octave-core *.oct *~

.PHONY: distclean
distclean: clean