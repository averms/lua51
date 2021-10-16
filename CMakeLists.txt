cmake_minimum_required(VERSION 3.16)

project(
  lua51
  LANGUAGES C
  VERSION 5.1.5
)

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
  message(FATAL_ERROR "In-source builds not allowed.")
endif()

include(GNUInstallDirs)

set(LUA_PROPS C_STANDARD 99 C_STANDARD_REQUIRED ON C_EXTENSIONS OFF)
set(LUA_DEFS)

option(WITH_LOADLIB
  "Allow Lua to 'require' C extension modules" OFF)
option(WITH_READLINE
  "Link with GNU readline to add history to the Lua interpreter" OFF)

if(CMAKE_C_COMPILER_ID STREQUAL Clang OR CMAKE_C_COMPILER_ID STREQUAL GNU)
  add_compile_options("-Wall" "-Wno-misleading-indentation")
  if(CMAKE_C_COMPILER_ID STREQUAL Clang)
    add_compile_options("-Wno-empty-body")
  endif()
endif()

if(UNIX)
  list(APPEND LUA_DEFS LUA_USE_POSIX)
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  list(APPEND LUA_DEFS _DEFAULT_SOURCE)
endif()

add_library(
  lua
  src/lapi.c
  src/lauxlib.c
  src/lbaselib.c
  src/lcode.c
  src/ldblib.c
  src/ldebug.c
  src/ldo.c
  src/ldump.c
  src/lfunc.c
  src/lgc.c
  src/linit.c
  src/liolib.c
  src/llex.c
  src/lmathlib.c
  src/lmem.c
  src/loadlib.c
  src/lobject.c
  src/lopcodes.c
  src/loslib.c
  src/lparser.c
  src/lstate.c
  src/lstring.c
  src/lstrlib.c
  src/ltable.c
  src/ltablib.c
  src/ltm.c
  src/lundump.c
  src/lvm.c
  src/lzio.c
)

set_target_properties(
  lua
  PROPERTIES ${LUA_PROPS}
  VERSION "${PROJECT_VERSION}"
  SOVERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}"
  PUBLIC_HEADER "src/lua.h;src/luaconf.h;src/lualib.h;src/lauxlib.h;etc/lua.hpp"
)
target_compile_definitions(lua PRIVATE ${LUA_DEFS})
target_include_directories(lua PUBLIC src)

find_library(MATH_LIB m)
if(MATH_LIB)
  target_link_libraries(lua PRIVATE "${MATH_LIB}")
endif()

if(WITH_LOADLIB)
  target_link_libraries(lua PRIVATE "${CMAKE_DL_LIBS}")
  if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    target_compile_definitions(lua PRIVATE LUA_DL_DYLD)
  elseif(WIN32)
    # LUA_DL_DLL is already defined, see lines 32 and 704 of luaconf.h
  elseif(UNIX)
    target_compile_definitions(lua PRIVATE LUA_USE_DLOPEN)
  else()
    message(FATAL_ERROR "loadlib will not work on this platform")
  endif()
endif()

add_executable(lua_exe src/lua.c)
set_target_properties(lua_exe PROPERTIES ${LUA_PROPS} OUTPUT_NAME lua5.1)
target_compile_definitions(lua_exe PRIVATE ${LUA_DEFS})
target_link_libraries(lua_exe PRIVATE lua)

if(WITH_LOADLIB)
  set_target_properties(lua_exe PROPERTIES ENABLE_EXPORTS ON)
endif()

if(WITH_READLINE)
  target_link_libraries(lua_exe PRIVATE readline)
  target_compile_definitions(lua_exe PRIVATE LUA_USE_READLINE)
endif()

# Only with static liblua
# add_executable(luac_exe src/print.c src/luac.c)
# set_target_properties(luac_exe PROPERTIES ${LUA_PROPS} OUTPUT_NAME luac5.1)
# target_compile_definitions(luac_exe PRIVATE ${LUA_DEFS})
# target_link_libraries(luac_exe PRIVATE lua)

install(
  TARGETS lua lua_exe
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/lua5.1
)
