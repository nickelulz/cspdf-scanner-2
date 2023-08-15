CC := gcc
BIN := bin
EXE := cspdf2-executable

# Source Files
SRC := $(wildcard src/**/*.c) \
	   $(wildcard src/*.c) \
	   $(wildcard src/**/**/*.c) \
	   $(wildcard src/**/**/**/*.c)

# Objects - all generated to bin/
OBJ := $(patsubst %.c, $(BIN)/%.o, ${SRC})
LIB_OBJ := $(BIN)/lib/log.o #lib/pdfio/libpdfio.so.1

CFLAGS := -O3 -g -Wall -Wextra -Wpedantic -DLOG_USE_COLOR -Ilib/log.c/src `pkg-config --cflags pdfio`
LDFLAGS := $(LIB_OBJ) -lm -lpthread `pkg-config --libs pdfio`

.PHONY: stats all clean

# Run post-compilation
run: stats all
	./$(BIN)/$(EXE)

# Compile the executable
all: dirs libs $(EXE)

# Generate the directories
dirs:
	mkdir -p ./$(BIN) ./$(BIN)/lib ./$(BIN)/src ./data ./out

# One-time dependencies
install_deps:
	cd lib/pdfio && sudo make install

# Line count statistics
stats:
	cd src && (find . -name '*.[ch]' -print0 | xargs -0 cat) | wc -l

libs:
	# Compile log.c library
	$(CC) -o $(BIN)/lib/log.o -c lib/log.c/src/log.c

$(EXE): $(OBJ)
	$(CC) -o $(BIN)/$(EXE) $^ $(LDFLAGS)

$(BIN)/%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -rf $(BIN) $(OBJ) $(LIBOBJ)