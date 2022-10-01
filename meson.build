project('lua', 'c',
  version: '5.1.5',
  license: 'MIT',
  meson_version: '>=0.56.0',
  default_options: [
    'warning_level=2',
    'buildtype=release',
    'c_std=c99',
    'strip=true',
    'b_pie=true',
  ]
)

version_parts = meson.project_version().split('.')

cc = meson.get_compiler('c')
# Get rid of warnings, at least on clang and gcc.
if cc.get_id() == 'gcc'
  add_project_arguments(
    '-Wno-misleading-indentation',
    '-Wno-implicit-fallthrough',
    '-Wno-address',
    language: 'c',
  )
elif cc.get_id() == 'clang'
  add_project_arguments(
    '-Wno-empty-body',
    '-Wno-misleading-indentation',
    language: 'c',
  )
endif

os = host_machine.system()

is_posix = os not in ['windows', 'emscripten', 'android']
if is_posix
  add_project_arguments('-DLUA_USE_POSIX', language: 'c')
endif

# Enable _longjmp and some POSIX functions.
if os == 'linux'
  add_project_arguments('-D_DEFAULT_SOURCE', language: 'c')
endif

lib_deps = [cc.find_library('m', required: false)]

if get_option('with_loadlib')
  dl_dep = cc.find_library('dl', required: false)
  lib_deps += [dl_dep]

  if os == 'darwin'
    add_project_arguments('-DLUA_DL_DYLD', language: 'c')
  elif os == 'windows'
    # LUA_DL_DLL is already defined, see lines 32 and 704 of luaconf.h
  elif is_posix
    add_project_arguments('-DLUA_USE_DLOPEN', language: 'c')
  else
    error('loadlib will not work on ' + os)
  endif
endif

lib = library(meson.project_name(),
  'src/lapi.c',
  'src/lauxlib.c',
  'src/lbaselib.c',
  'src/lcode.c',
  'src/ldblib.c',
  'src/ldebug.c',
  'src/ldo.c',
  'src/ldump.c',
  'src/lfunc.c',
  'src/lgc.c',
  'src/linit.c',
  'src/liolib.c',
  'src/llex.c',
  'src/lmathlib.c',
  'src/lmem.c',
  'src/loadlib.c',
  'src/lobject.c',
  'src/lopcodes.c',
  'src/loslib.c',
  'src/lparser.c',
  'src/lstate.c',
  'src/lstring.c',
  'src/lstrlib.c',
  'src/ltable.c',
  'src/ltablib.c',
  'src/ltm.c',
  'src/lundump.c',
  'src/lvm.c',
  'src/lzio.c',
  dependencies: lib_deps,
  version: meson.project_version(),
  # SO version of 5.1. This is what every Linux distro does.
  soversion: version_parts[0] + '.' + version_parts[1],
  implicit_include_directories: false,
)
lua_dep = declare_dependency(
  link_with: lib, include_directories: include_directories('src')
)
meson.override_dependency(meson.project_name(), lua_dep)

interpreter_deps = []
interpreter_defines = []
if get_option('with_readline')
  readline_dep = dependency('readline', required: true)
  interpreter_deps += [readline_dep]
  interpreter_defines += ['-DLUA_USE_READLINE']
endif

we_are_static = get_option('default_library') == 'static'

lua_exe = executable('lua5.1',
  'src/lua.c',
  c_args: interpreter_defines,
  dependencies: [lua_dep] + interpreter_deps,
  export_dynamic: get_option('with_loadlib'),
  install: we_are_static,
  implicit_include_directories: false,
)

if we_are_static
  luac_exe = executable('luac5.1',
    'src/luac.c',
    'src/print.c',
    dependencies: lua_dep,
    install: true,
    implicit_include_directories: false,
  )
endif
