#!/usr/bin/env bash
prefix="$1"
exec_prefix=${prefix}
includedir=${prefix}/include
libdir=${exec_prefix}/lib

echo -e "prefix=${prefix}\nexec_prefix=${prefix}\nincludedir=${prefix}/include\nlibdir=${exec_prefix}/lib\n$(cat cuppa.pc.in)" > cuppa.pc
