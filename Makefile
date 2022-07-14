# TODO Do something with this export LD_LIBRARY_PATH="./bin/lib;$LD_LIBRARY_PATH"
CC=gcc
BINFLR=bin/

# Main Library
CFLAGS=-Wall -c -Iinclude/
LFLAGS=-lm -shared
TARGET=libosu.so

all: BINFLR=bin/lib/
all: CFLAGS+=-fPIC -g
all: $(TARGET)

%.o: %.c | $(BINFLR)
	$(CC) $(CFLAGS) -o $(BINFLR)$(notdir $@) $<

$(TARGET): $(addsuffix .o, $(basename $(shell find include/ -type f -name "*.h" | grep -Po '(?<=include/).*' | sed 's/^/src\//')))
	$(CC) -o $(BINFLR)$@ $(addprefix $(BINFLR), $(notdir $^)) $(LFLAGS)

# Test Executable
LFLAGS_TEST=-g -Iinclude/ -Lbin/lib/ -losu
TARGET_TEST=$(shell find test/*.c -type f)

test: BINFLR=bin/exe/
test: $(basename $(TARGET_TEST))

.c:
	$(CC) -o $(BINFLR)$(addprefix test_, $(notdir $@)) $^ $(LFLAGS_TEST)

# Make bin/ folder
$(BINFLR):
	mkdir -p $@/lib $@/exe

# Clean up
clean:
	rm -rf $(BINFLR)lib/* $(BINFLR)exe/*