
set -xe

CFLAGS="-Wall -Wextra -Wpedantic -O2 -o main"
CC="gcc"
$CC $CFLAGS main.c
