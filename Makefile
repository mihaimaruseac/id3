.PHONY: all clean test doc purge tags

TARGET = ./id3
CFLAGS = -Wall -Wextra -g -O0 -Wno-unused
LDFLAGS = -lm
OBJS = id3.o globals.o id3learn.o

all: $(TARGET)

$(TARGET): $(OBJS)

clean:
	$(RM) $(OBJS) $(TARGET)

purge:
	$(RM) -r $(OBJS) $(TARGET) tags html

test_learn: $(TARGET)
	$(TARGET) l tests/1/atribute.txt tests/1/invatare.txt tests/1/dump.txt

doc:
	doxygen
	firefox html/index.html &

tags:
	ctags *

