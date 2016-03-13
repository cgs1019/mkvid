# use pkg-config for getting CFLAGS and LDLIBS
FFMPEG_LIBS=    libavdevice                        \
                libavformat                        \
                libavfilter                        \
                libavcodec                         \
                libswresample                      \
                libswscale                         \
                libavutil                          \

CFLAGS += -Wall -O2 -g -std=c99
CFLAGS += -I /usr/local/include
CFLAGS += -L /usr/local/lib
CFLAGS := $(shell /usr/bin/pkg-config --cflags $(FFMPEG_LIBS)) $(CFLAGS)

LDLIBS := $(shell /usr/bin/pkg-config --libs $(FFMPEG_LIBS)) $(LDLIBS)

# the following examples make explicit use of the math library
decoding_encoding: LDLIBS += -lm
muxing:            LDLIBS += -lm

.phony: all clean-test clean

SRCS=    encode_example     \
				 mux_example


OBJS=$(addsuffix .o,$(SRCS))

all: $(OBJS) $(SRCS)

clean-test:
	$(RM) test*.pgm test.h264 test.mp2 test.sw test.mpg

clean: clean-test
	$(RM) $(EXAMPLES) $(OBJS)
