C = gcc -Wall -Werror -Wextra
BUILD_PATH = ../build/
SRCS = graph.c parcer.c pl_math.c paint.c stack.c -lm

all:
	$(C) -o $(BUILD_PATH)Final $(SRCS)

load:
	valgrind --tool=memcheck --leak-check=yes ./$(BUILD_PATH)Final

clean:
	rm -rf $(BUILD_PATH)*

run: clean all load
