
# nem2-sdk-cpp

nem2-sdk-cpp is a C++ client library for Catapult API.

## Prerequisites

* C++ compliler with C++17 support
* CMake 3.8 or higher
* [Conan](https://conan.io)

## Common notes

Following variables can be used to control build process:
* NEM2_SDK_BUILD_STATIC - build static version of library (default - OFF)
* NEM2_SDK_BUILD_TESTS - set to ON if you want to build tests (default - OFF)
* NEM2_SDK_BUILD_EXAMPLES - set to ON if you want to build the example programs (default - OFF)
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

You can open _nem2-sdk-cpp.sln_ file with VS and build from its GUI or use Microsoft Build Engine:
```
msbuild.exe /p:Configuration=Release ALL_BUILD.vcxproj
msbuild.exe INSTALL.vcxproj
```

## Usage

Use the following command to find shared or static version of NEM2 SDK library:
```
find_package(nem2-sdk-cpp)
```
or
```
find_package(nem2-sdk-cpp-static)
```

NEM2 SDK package config exports the following target in _NEM2::_ namespace:
* nemcpp

You can also use this variables (not recommended):
* nem2-sdk-cpp_INCLUDE_DIR
* nem2-sdk-cpp_LIBRARY
