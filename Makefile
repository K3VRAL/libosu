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
	$(CC) $(CFLAGS) -o $(BINFLR)$(shell echo "$@" | perl -pe 's/((\/.|^.)\K).*?(?=\/)//g' | perl -pe 's/\//_/g') $(shell echo "$<")

$(TARGET): $(shell find include/ -type f -name "*.h" | perl -pe 's/include\///g' | perl -pe 's/\..*/.o/g' | perl -pe 's/^/src\//g')
	$(CC) -o $(BINFLR)$@ $(addprefix $(BINFLR), $(notdir $^)) $(LFLAGS)
# TODO					 ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

# Test Executable
LFLAGS_TEST=-g -Iinclude/ -losu
TARGET_TEST=$(shell find test/*.c -type f | perl -pe 's/\..*//g')

test: BINFLR=bin/
test: $(TARGET_TEST)

.c:
	$(CC) -o $(BINFLR)$(shell echo $@ | perl -pe 's/.+\//test_/g') $^ $(LFLAGS_TEST)

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

cleanlib:
	rm -rf $(BINFLR)lib/*