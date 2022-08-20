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
	$(shell echo 'lol $@ $<')
# $(CC) $(CFLAGS) -o $(BINFLR)$(shell echo $@ | perl -pe 's/.+\///') $(shell echo $< | perl -pe 's/((\/.|^.)\K).*?(?=\/)//' | perl -pe 's/\//_/')

$(TARGET): $(shell find include/ -type f -name "*.h" | perl -pe 's/include\///' | perl -pe 's/\..*//' | perl -pe 's/^/src\//')
	$(CC) -o $(BINFLR)$@ $(addprefix $(BINFLR), $(notdir $^)) $(LFLAGS)

# Test Executable
LFLAGS_TEST=-g -Iinclude/ -losu
TARGET_TEST=$(shell find test/*.c -type f | perl -pe 's/\..*//')

test: BINFLR=bin/
test: $(TARGET_TEST)

.c:
	$(CC) -o $(BINFLR)$(shell echo $@ | perl -pe 's/.+\//test_/') $^ $(LFLAGS_TEST)

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
	$(shell mkdir -p $@/lib)

# Clean up
clean:
	rm -rf $(BINFLR)*