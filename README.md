
[![License](https://img.shields.io/badge/License-Apache%202.0-blue.svg)](https://opensource.org/licenses/Apache-2.0)

# cpp-xpx-chain-sdk

Official ProximaX Sirius Blockchain SDK Library in C++.

## Prerequisites ##

* C++ compliler with C++17 support
* CMake 3.8 or higher
* [Conan](https://conan.io)

## Common notes ##

Following variables can be used to control build process:
* XPX_CHAIN_SDK_BUILD_STATIC - build static version of library (default - OFF)
* XPX_CHAIN_SDK_BUILD_TESTS - set to ON if you want to build tests (default - OFF)
* XPX_CHAIN_SDK_BUILD_EXAMPLES - set to ON if you want to build the example programs (default - OFF)
* other well-known CMake variables (CMAKE_INSTALL_PREFIX, CMAKE_BUILD_TYPE, etc.)

## Building ##

Prepare build with following instructions:

```
mkdir build
cd build
cmake ..
```

### Building with *nix Make ###

```
make -j 4
make install
```

### Building with Visual Studio ###

You can open _cpp-xpx-chain-sdk.sln_ file with VS and build from its GUI or use Microsoft Build Engine:
```
msbuild.exe /p:Configuration=Release ALL_BUILD.vcxproj
msbuild.exe INSTALL.vcxproj
```

## Usage ##

Use the following command to find shared or static version of XPX-CHAIN SDK library:
```
find_package(cpp-xpx-chain-sdk)
```
or
```
find_package(cpp-xpx-chain-sdk-static)
```

XPX-CHAIN SDK package config exports the following target in _XPX_CHAIN::_ namespace:
* xpxchaincpp

You can also use this variables (not recommended):
* cpp-xpx-chain-sdk_INCLUDE_DIR
* cpp-xpx-chain-sdk_LIBRARY

## Core Contributors
 + [@pananton13](https://github.com/pananton13)
 + [@vulfke482](https://github.com/vulfke482)
 + [@eugene-babichenko](https://github.com/eugene-babichenko)
 
## Contribution ##

We'd love to get more people involved in the project. Please feel free to [raise issue](https://github.com/proximax-storage/cpp-xpx-chain-sdk/issues/new) or PR and we'll review your contribution.
    
## License ##

Project is licensed under Apache License Version 2.0. See [LICENSE](https://github.com/proximax-storage/cpp-xpx-chain-sdk/blob/master/LICENSE) file for details.

<p align="right"><i><sub>Copyright (c) 2019 ProximaX Limited</sub></i></p>
