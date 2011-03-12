.PHONY: all clean test doc purge tags

TARGET = ./id3
CFLAGS = -Wall -Wextra -g -O0 -Wno-unused
LDFLAGS = -lm
OBJS = id3.o

all: $(TARGET)

$(TARGET): $(OBJS)

clean:
	rm -f $(OBJS) $(TARGET)

purge:
	rm -rf $(OBJS) $(TARGET) tags html

test: $(TARGET)
	$(TARGET)

doc:
	doxygen
	firefox html/index.html &

tags:
	ctags *

