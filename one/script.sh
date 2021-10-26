#!/bin/sh
# Run ccombine[1] on any of lua_*.c to create a portable amalgamation you can drop
# anywhere.
# 
# [1]: https://github.com/averms/ccombine

gcc -std=c99 -Os -Wall -Wextra -Wpedantic \
    -Wno-implicit-fallthrough -Wno-misleading-indentation \
    -I ../src -D_DEFAULT_SOURCE -DLUA_USE_POSIX -DMAKE_LUA one_lua.c -lm -o lua.out
