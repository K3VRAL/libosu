all code is based on [ppy's osu!lazer project](https://github.com/ppy/osu). ppy pls don't sue me

# libosu

libosu is a library meant to implement the source code of osu!lazer to C. This is due to the fact that it writing code in C using C# libraries either seems to be difficulty or impossible (due to how C works), as there is a lack of information on how to do so. This library only aims to code the functionality and allow for the user's to output the data; not coding the GUI game itself.

## Table of Contents

**[Installation](#install)**

**[Documentation](#doc)**

<a name="install"></a>

## Installation

The tools you will also need before you can do anything is `make`, `gcc`, and `pkg-config`.

Once you have the necessary tools and the project has been cloned, in your terminal while still in the same folder location of this project's `Makefile`, write `make`, to compile the project to a library, then `sudo make install`, to install the library to `/usr/lib/` safely.

If you wish to uninstall the program, do sudo make uninstall and that should get rid of everything.

<a name="doc"></a>

## Documentation

TODO