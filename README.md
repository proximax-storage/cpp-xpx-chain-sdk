
# xpx-chain-sdk-cpp

xpx-chain-sdk-cpp is a C++ client library for Catapult API.

## Prerequisites

* C++ compliler with C++17 support
* CMake 3.8 or higher
* [Conan](https://conan.io)

## Common notes

Following variables can be used to control build process:
* XPX_CHAIN_SDK_BUILD_STATIC - build static version of library (default - OFF)
* XPX_CHAIN_SDK_BUILD_TESTS - set to ON if you want to build tests (default - OFF)
* XPX_CHAIN_SDK_BUILD_EXAMPLES - set to ON if you want to build the example programs (default - OFF)
* other well-known CMake variables (CMAKE_INSTALL_PREFIX, CMAKE_BUILD_TYPE, etc.)

## Building

Prepare build with following instructions:

```
mkdir build
cd build
cmake ..
```

### Building with *nix Make

```
make -j 4
make install
```

### Building with Visual Studio

You can open _xpx-chain-sdk-cpp.sln_ file with VS and build from its GUI or use Microsoft Build Engine:
```
msbuild.exe /p:Configuration=Release ALL_BUILD.vcxproj
msbuild.exe INSTALL.vcxproj
```

## Usage

Use the following command to find shared or static version of XPX-CHAIN SDK library:
```
find_package(xpx-chain-sdk-cpp)
```
or
```
find_package(xpx-chain-sdk-cpp-static)
```

XPX-CHAIN SDK package config exports the following target in _XPX_CHAIN::_ namespace:
* xpxchaincpp

You can also use this variables (not recommended):
* xpx-chain-sdk-cpp_INCLUDE_DIR
* xpx-chain-sdk-cpp_LIBRARY
