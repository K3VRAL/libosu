CC=gcc
BINFLR=bin/

# Main Library
CFLAGS=-Wall -c -Iinclude/
LFLAGS=-lm -shared
TARGET=libosu.so

all: BINFLR=bin/lib/
all: CFLAGS+=-fPIC -g
all: $(TARGET)

perlModify=perl -pe "s/^src\//osu\//g" | perl -pe "s/((\/.|^.)\K).*?(?=\/)//g" | perl -pe "s/\/(?=[^.]*\/)//g" | perl -pe "s/\//_/g"
#		   ^^^^^^^^^^^^^^^^^^^^^^^^^^    ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^   ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^   ^^^^^^^^^^^^^^^^^^^
#		   ^In the beginning of the file's path string, we want to remove src/ and replace it with osu/ mainly because that's how the project structure is framed as. Eg: src/path/to/file.* => osu/path/to/file.*
#										 ^We keep the first character of each word within the /'s but remove everything else. We also don't modify the last path of the string. Eg: osu/path/to/file.* => o/p/t/file.*
#													                             ^We remove all of the /'s except for the last occurrence. Eg: o/p/t/file.* => opt/file.*
#																				                                 ^We turn all /'s that remain (should only be one) and turn it into a _. Eg: opt/file.* => opt_file.*

%.o: %.c | $(BINFLR)
	$(CC) $(CFLAGS) -o $(BINFLR)$(shell echo "$@" | $(perlModify)) $(shell echo "$<")

#																						\/Changes the file extension from anything (should be .h) to .o . Eg: src/file/to/path.h => src/file/to/path.o
#													   \/The file path removes include/ and changes it to src/. Eg: include/file/to/path.h => src/file/to/path.h
#				   \/Before we can start doing anything; we first need to find every single file in the directory that can be compiled. It would make send to find every .c file since they are the ones being compiled; but I believe for the sake of transparency (since the header files are the ones being used in order to use the library), the project structure is made such that for every .h header file that exists; there is a dedicated compiled file associated with it
#				   \/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/    \/\/\/\/\/\/\/\/\/\/\/\/\/\/\/   \/\/\/\/\/\/\/\/\/\/\/
$(TARGET): $(shell find include/ -type f -name "*.h" | perl -pe "s/^include\//src\//g" | perl -pe "s/\..*/.o/g")
	$(CC) -o $(BINFLR)$@ $(foreach file, $^, $(shell echo "$(file)" | $(perlModify) | (echo -n $(BINFLR) && cat))) $(LFLAGS)
#						 ^^^^^^^^^^^^^^^^^^			 ^^^^^^^^^^^^^^   ^^^^^^^^^^^^^   ^^^^^^^^^^^^^^^^^^^^^^^^^^
#						 ^We need to go through each file which we use linux's shell commands as they make the process of modifying the strings more specific than GNUmake's core commands
#													 ^We need to echo the file's path since piping requires the data to be put to stdout
#																					  ^We prepend the bin folder variable to the echo. Eg: opt_file.o => bin/opt_file.o

# Test Executable
# $(shell pkg-config --libs osu)
LFLAGS_TEST=-g -Iinclude/ -losu
TARGET_TEST=$(shell find test/*.c -type f | perl -pe "s/\..*//g")

test: BINFLR=bin/
test: $(TARGET_TEST)

.c:
	$(CC) -o $(BINFLR)$(shell echo "$@" | perl -pe "s/.+\//test_/g") $^ $(LFLAGS_TEST)

# TODO Change ./include and add to pkg-config
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
	$(shell rm -rf $(BINFLR)*)

cleanlib:
	$(shell rm -rf $(BINFLR)lib/*)