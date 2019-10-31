# LuaHook-NG

[![Build status](https://ci.appveyor.com/api/projects/status/3dapx2rm8u0fj05e?svg=true)](https://ci.appveyor.com/project/Olipro/luahook-ng)

## About

LuaHook-NG is a library designed to facilitate hooking into applications which
utilise Lua.

## Building

The project is CMake based. a JSON file is included for the benefit of Visual
Studio's CMake support; it can also be used as a guideline for the appropriate
compiler, assembler and linker arguments that should be used when invoking
CMake if you don't have Visual Studio 2019.

## Architecture

The primary responsibilities are separated into two parts:

### The Injector

The hook is responsible for getting into the process space of the target app.
Currently only one such hook exists which acts as a shim for the Windows OS
DLL IPHLPAPI.dll

The injector takes care of loading LuaHook-NG which then takes care of hooking.

### The Hook

The hook consists of a generic set of interfaces and some generic code code.

These interfaces are designed to be used to bring together implementation code
that handles a specific platform, application and, if needed, runtime-patching.

When adding a new OS, you will need to provide an entry point for it. Currently
there is a dllmain provided for Windows only. You should also implement a loader
that will load client libraries (code that will use the Lua API and interact
with the targeted application)

There is currently an implementation for PAYDAY2 on x86 Windows, PAYDAY1 will
also be supported shortly. In order to use the hook, both IPHLPAPI.dll and
LuaHook-NG.dll need to reside in the same directory as PAYDAY2. Your client
libraries should live in a subfolder called LuaHook-NG.

## Client Libraries

LuaHook-NG does not interfere with target applications beyond establishing the
necessary hook points (if any). It is intended that you build separate library
files which then reside in the LuaHook-NG folder. These will then be dynamically
loaded at runtime by LuaHook-NG.

## API

When building a client library, it is advisable to look at the source code of
the ones bundled with this code repository. In any case though, here is a guide
to the general procedure:

1. `#include <LuaHookNG.h>` - this provides all necessary types and functions
   inside the `LuaHookNG` namespace;
2. Decide which points you need to hook and create a class or classes that
   implement one or more of the interfaces defined in
  `LuaHook-NG/include/interface`.
3. 
   a. Create an instance of `LuaHookNG::LuaAppLink` passing a reference to your
      class as a constructor argument. For convenience, you can use
      `LuaAppLink::RegisterMultiple` if you have a single class that implements
      multiple interfaces. You are **required** to ensure that your class instance
      is valid for the duration of `LuaAppLink`'s lifetime.
	b. When the `LuaAppLink` instance goes out of scope, the client registered
       to it will no longer be registered.

Please note that you **must not** construct or destruct a `LuaAppLink` from
within your handler client. This will result in a double-acquire of a protective
mutex within LuaHook-NG which results in *Undefined Behaviour*. You should create
a signalling mechanism between your handler client and the rest of your app which
can then safely unregister your handler.

## Lua API

The Lua API is designed to be usable in the same way you would expect to use
a normal Lua API. However, this may not be possible for all applications.

See the README.md in the folder of the application you are interested in for
info on any limitations or unimplemented Lua functions - for example,
`src/windows/x86/PAYDAY2/README.md`. Please note that unimplemented Lua
functions will always throw `std::runtime_error` - they may also present a
visible dialog, depending on platform.

### Caveats

Please do note that the sizes of `lua_Number` and `lua_Integer` will always be
`double` and `ptrdiff_t` respectively. However, you need to be aware of your
target application if you intend to rely on these values. For target apps which
use smaller size types (such as PAYDAY1 using `float`) values you pass in will
be narrowed, values you receive will have been extended.

Similarly, Lua implementations may vary - if you attempt to load in standard
Lua bytecode into a LuaJIT app, it will fail and vice versa.

## Versioning

LuaHookNG implements a `Version()` function which you can call to determine the
hook's release.

The project uses *semantic* versioning and you can consequently expect that
a change in the major version may break compatibility, including the ability
to successfully load your client library.

A change in the minor version will not break any correctly used functionality,
but new features have been added.

A change in the patch version will also not break any correctly use
functionality and only denotes bug fixes.
