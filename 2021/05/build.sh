
set -xe

CFLAGS="-Wall -g -Wextra -Wpedantic -O2 -o main"
CC="gcc"
$CC $CFLAGS main.c
