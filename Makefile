.PHONY: all clean tests doc purge tags

TARGET = ./id3
CFLAGS = -Wall -Wextra -g -O0
LDFLAGS = -lm
OBJS = id3.o globals.o id3learn.o id3missing.o id3math.o id3graph.o id3test.o

all: $(TARGET)

$(TARGET): $(OBJS)

clean:
	$(RM) $(OBJS) $(TARGET)

purge:
	$(RM) -r $(OBJS) $(TARGET) tags html

tests:
	@echo "Running tests..."
	@tests/runAll.sh
	@echo "Testing done"

doc:
	@echo "Building documentation..."
	@doxygen > /dev/null
	@echo "Starting browser in background..."
	@firefox html/index.html &

tags:
	ctags *

