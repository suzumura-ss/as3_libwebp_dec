# base library
LIBNAME = libwebp-0.3.1

DEC_SRC  = alpha.c buffer.c frame.c idec.c io.c layer.c quant.c tree.c vp8.c vp8l.c webp.c
UTIL_SRC = bit_reader.c color_cache.c filters.c huffman.c quant_levels_dec.c rescaler.c \
           thread.c utils.c bit_writer.c huffman_encode.c quant_levels.c 
DSP_SRC  = cpu.c dec.c dec_neon.c dec_sse2.c lossless.c upsampling.c upsampling_neon.c \
           upsampling_sse2.c yuv.c enc.c enc_neon.c enc_sse2.c


BASE = $(LIBNAME)/src
LIBSRC = $(foreach s,$(DEC_SRC),$(BASE)/dec/$s) \
         $(foreach s,$(UTIL_SRC),$(BASE)/utils/$s) \
         $(foreach s,$(DSP_SRC),$(BASE)/dsp/$s)
LIBOBJ = $(DEC_SRC:.c=.o) $(UTIL_SRC:.c=.o) $(DSP_SRC:.c=.o)
DECLIB = libWebpDec.a

TARGET = libWebpDec.swc

# wrapper
SRC = main.cpp as3api.cpp
OBJ = $(foreach b,$(basename $(SRC)), $s.o)


# build options
CFLAGS = $(BASE_CFLAGS) -O3 -DNDEBUG -DWEBP_HAVE_PNG -DWEBP_HAVE_JPEG -DWEBP_HAVE_TIFF -DWEBP_USE_THREAD \
         -Wextra -Wshadow -Wall -I$(BASE)/webp -I$(BASE)/dec -I$(BASE)/utils -I$(BASE)/dsp


.PHONY: all
all: $(TARGET)

.PHONY: clean
clean:
	rm -f *.swf *.swc *.as3 *.abc *.o
	rm -f $(DECLIB)
	rm -rf $(LIBNAME) $(LIBNAME).tar.gz


$(TARGET): $(SRC) $(DECLIB)
	"$(FLASCC)/usr/bin/gcc" $(CFLAGS) -O4 $(SRC) $(DECLIB) -emit-swc=info.smoche.libWebp.Decode -lFlash++ -lAS3++ -o $@

$(DECLIB): check extract
	"$(FLASCC)/usr/bin/gcc" $(CFLAGS) -O4 -c $(LIBSRC)
	"$(FLASCC)/usr/bin/ar" -r -c $(DECLIB) $(LIBOBJ)
	@rm -f $(LIBOBJ) 

.PHONY: extract
extract:
	[ -e $(LIBNAME) ] || wget https://webp.googlecode.com/files/$(LIBNAME).tar.gz
	[ -e $(LIBNAME) ] || tar xf $(LIBNAME).tar.gz

include Makefile.common

