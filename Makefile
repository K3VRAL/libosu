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
LFLAGS_TEST=-g -Iinclude/ -losu
TARGET_TEST=$(shell find test/*.c -type f)

test: BINFLR=bin/exe/
test: $(basename $(TARGET_TEST))

.c:
	$(CC) -o $(BINFLR)$(addprefix test_, $(notdir $@)) $^ $(LFLAGS_TEST)

# TODO Change ./include
# Install
install:
	$(shell rm -rf /usr/local/lib/$(TARGET))
	$(shell cp ./bin/lib/$(TARGET) /usr/local/lib/$(TARGET))
	$(shell rm -rf /usr/lib/$(TARGET))
	$(shell ln -s /usr/local/lib/$(TARGET) /usr/lib/)
	$(shell rm -rf /usr/local/include/osu)
	$(shell cp -r ./include /usr/local/include/osu)
	$(shell rm -rf /usr/include/osu)
	$(shell ln -s /usr/local/include/osu /usr/include/)

# Uninstall
uninstall:
	$(shell rm -rf /usr/local/lib/$(TARGET))
	$(shell rm -rf /usr/lib/$(TARGET))
	$(shell rm -rf /usr/local/include/osu)
	$(shell rm -rf /usr/include/osu)

# Make bin/ folder
$(BINFLR):
	mkdir -p $@/lib $@/exe

# Clean up
clean:
	rm -rf $(BINFLR)lib/* $(BINFLR)exe/*