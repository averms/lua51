/*
* one_luac.c -- Lua bytecode compiler in a single file (Lua 5.1)
* Not from lua.org/extras. Adapted from all.c
*/

/* setup for luaconf.h */
#define luaall_c

/* core -- used by all */
#include "lapi.c"
#include "lcode.c"
#include "ldebug.c"
#include "ldo.c"
#include "ldump.c"
#include "lfunc.c"
#include "lgc.c"
#include "llex.c"
#include "lmem.c"
#include "lobject.c"
#include "lopcodes.c"
#include "lparser.c"
#include "lstate.c"
#include "lstring.c"
#include "ltable.c"
#include "ltm.c"
#include "lundump.c"
#include "lvm.c"
#include "lzio.c"

/* auxiliary library -- used by all */
#include "lauxlib.c"

/* luac */
#include "print.c"
#include "luac.c"
