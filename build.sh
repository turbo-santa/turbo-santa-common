#! /bin/bash

echo "Removing old build files..."
rm aclocal.m4 > /dev/null
rm Makefile > /dev/null
rm Makefile.in > /dev/null
rm configure > /dev/null

echo "Building..."
aclocal && autoconf && automake --add-missing && ./configure && make


