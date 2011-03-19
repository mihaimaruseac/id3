.PHONY: all clean test doc purge tags

TARGET = ./id3
CFLAGS = -Wall -Wextra -g -O0 -Wno-unused
LDFLAGS = -lm
OBJS = id3.o globals.o id3learn.o id3missing.o id3math.o id3graph.o id3test.o
DEBUG = valgrind --leak-check=full --show-reachable=yes

all: $(TARGET)

$(TARGET): $(OBJS)

clean:
	$(RM) $(OBJS) $(TARGET)

purge:
	$(RM) -r $(OBJS) $(TARGET) tags html

test_learn: $(TARGET)
	$(DEBUG) $(TARGET) l tests/1/atribute.txt tests/1/invatare.txt tests/1/dump.txt

test_graph: $(TARGET)
	$(DEBUG) $(TARGET) g -gscheme tests/1/dump.txt -

test_classify: $(TARGET)
	$(DEBUG) $(TARGET) c tests/1/dump.txt tests/1/test.txt -

doc:
	doxygen > /dev/null
	firefox html/index.html &

tags:
	ctags *

