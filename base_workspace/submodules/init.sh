#! /bin/bash

cd glog
./configure
make
cd ..

cd googletest
autoreconf -fvi
./configure
make
cd ..
