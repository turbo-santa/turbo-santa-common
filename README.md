Turbo Santa
===========
A platform independent gameboy emulator.

About
-----
Turbo Santa aims to put a gameboy emulator on any platform that supports Java.
This repo contains all the C++ code that each platform shares.

Installation
------------
In order to build this you must have:
  - git
  - g++/clang (with support for C++11)
  - ncurses development headers
  - [bazel](https://github.com/google/bazel)

Clone the repo:

    $ git clone https://github.com/turbo-santa/turbo-santa-common.git

Go into the project:

    $ cd turbo-santa-common

Get the submodules:

Go to your bazel base\_workspace and copy the symlinks that bazel needs to
function:

    $ cd base_workspace
    $ cp -R PATH/TO/BAZEL/base_workspace/* .

Now all you have to do is build Turbo Santa:

    $ cd .. # After typing this you should be in base_workspace
    $ bazel build //cc/backend:turbo

If everything works correctly you just build the Turbo Santa backend! The
backend may be run with:

    $ bazel-bin/cc/backend/turbo path/to/rom.gb 2> out.txt

You will want to use the `2> out.txt` at the end otherwise logs will be printed
all over your screen.

We now also have a java test UI which can be built with:

    $ bazel build //java/com/turbosanta/backend:main

Which may be run with:

    $ bazel-bin/java/com/turbosanta/backend/main -args path/to/rom.gb 2> out.txt
