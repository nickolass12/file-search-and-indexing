# Compiler setup.
CC=gcc
CFLAGS=-Wall -Wextra -std=c99

TARGETS=search_index

build: $(TARGETS)
$(TARGETS): main.c operations.c structsFunctions.c memoryUse.c structs.h
	$(CC) $(CFLAGS) main.c operations.c structsFunctions.c memoryUse.c -o $(TARGETS)

run: build
	./$(TARGETS)

pack:
	zip -FSr FileIndexing.zip README.md Makefile main.c operations.c structsFunctions.c memoryUse.c structs.h

clean:
	rm -f $(TARGETS)

.PHONY: build run pack clean