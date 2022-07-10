CC=gcc
CFLAGS=-Wall -c -g -I./include
BINFLR=bin/

# Main Library
LFLAGS=-shared
TARGET=libosu.so

all: BINFLR=bin/lib/
all: CFLAGS+=-fPIC
all: $(TARGET)

%.o: src/*/%.c include/*/%.h | $(BINFLR)
	$(CC) $(CFLAGS) -o $(BINFLR)$(notdir $@) $<

$(TARGET): $(addsuffix .o, $(basename $(notdir $(shell find include/ -type f)))) # FIX THIS - BEFORE `$(wildcard include/*/*.h)` OR `$(wildcard include/*.h)`
	$(CC) -o $(BINFLR)$@ $(addprefix $(BINFLR), $(notdir $^)) $(LFLAGS)

# Test Executable
LFLAGS_TEST=-losu -g
TARGET_TEST=$(wildcard test/*.c)

test: BINFLR=bin/exe/
test: $(basename $(TARGET_TEST))

.c:
	$(CC) -o $(BINFLR)$(addprefix test_, $(notdir $@)) $^ $(LFLAGS_TEST)

# Make bin/ folder
$(BINFLR):
	[ -d $(BINFLR) ] || mkdir -p $(BINFLR)

# Clean up
clean:
	rm -rf $(BINFLR)