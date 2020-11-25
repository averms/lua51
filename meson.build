project(
  'lua51', 'c',
  license: 'MIT',
  meson_version: '>=0.56.0',
  version: '5.1.5',
  default_options: [
    'warning_level=2',
    'buildtype=release',
    'c_std=c99',
    'strip=true',
  ]
)

version_parts = meson.project_version().split('.')

cc = meson.get_compiler('c')
# Get rid of warnings, at least on clang and gcc.
if cc.get_id() == 'gcc'
  add_project_arguments(
    '-Wno-misleading-indentation',
    '-Wno-implicit-fallthrough',
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

lua_lib_deps = [cc.find_library('m', required: false)]

if get_option('with_loadlib')
  dl_dep = cc.find_library('dl', required: false)
  lua_lib_deps += [dl_dep]
  if os == 'darwin'
    add_project_arguments('-DLUA_DL_DYLD', language: 'c')
  elif os == 'windows'
    # LUA_DL_DLL is already defined, see lines 32 and 704 of luaconf.h
  elif is_posix
    add_project_arguments('-DLUA_USE_DLOPEN', language: 'c')
  else
    error('loadlib will not work on this platform: ' + os)
  endif
endif

lua_lib = library('lua',
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
  dependencies: lua_lib_deps,
  version: meson.project_version(),
  # SOVERSION of 5.1. This is what every Linux distro does.
  soversion: version_parts[0] + '.' + version_parts[1],
  implicit_include_directories: false,
)
lua_dep = declare_dependency(
  link_with: lua_lib, include_directories: include_directories('src')
)
meson.override_dependency(meson.project_name(), lua_dep)

lua_exe_deps = [lua_dep]
lua_exe_defs = []
if get_option('with_readline')
  readline_dep = dependency('readline', required: true)
  lua_exe_deps += [readline_dep]
  lua_exe_defs += ['-DLUA_USE_READLINE']
endif

lua_exe = executable('lua5.1',
  'src/lua.c',
  c_args: lua_exe_defs,
  dependencies: lua_exe_deps,
  export_dynamic: get_option('with_loadlib'),
  implicit_include_directories: false,
  install: true,
)

if get_option('default_library') == 'static'
  luac_exe = executable('luac5.1',
    'src/luac.c', 'src/print.c',
    dependencies: lua_dep,
    implicit_include_directories: false,
  )
endif
