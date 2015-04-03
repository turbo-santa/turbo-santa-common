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
  - autotools (I am sorry)
  - libtool (I am very sorry)

Clone the repo:

    $ git clone https://github.com/google/bazel.git

Go into the project:

    $ cd turbo-santa-common

Get the submodules:

    $ git submodule update --init

Go to base\_workspace and copy the symlinks that bazel needs to function:

    $ cd base_workspace
    $ cp -R PATH/TO/BAZEL/base_workspace/* .

Compile the submodules:

    $ cd submodules
    $ ./init.sh

Now all you have to do is build Turbo Santa:

    $ cd .. # After typing this you should be in base_workspace
    $ blazel build //backend:turbo

If everything works correctly you just build the Turbo Santa backend!
