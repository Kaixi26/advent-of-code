
set -xe

CFLAGS="-Wall -ggdb -g -Wextra -Wpedantic -O2 -o main"
CC="gcc"
$CC $CFLAGS main.c
