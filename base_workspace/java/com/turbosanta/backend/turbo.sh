#! /bin/sh

BASEDIR=$(dirname $0)
LD_PRELOAD=/usr/lib/jvm/java-8-oracle/jre/lib/amd64/libjsig.so

LD_PRELOAD=$LD_PRELOAD $BASEDIR/main "$@"
