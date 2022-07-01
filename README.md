
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

## Building on Linux ##

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

## Building on Windows ##

### Install CMake ###
Download and install the latest CMake from https://cmake.org/download/ using the Windows x64 Installer.  
Make sure to select the option to add CMake bin directory to PATH in system variables.

### Install Conan ###
Download and install Conan from https://conan.io/downloads.html  
Add C:\Program Files\Conan\conan to PATH in system variables.

### Install MinGW-W64 ###
If you do not have MinGW-W64 installed, download the 12.1.0 MSVCRT runtime version from here https://winlibs.com/  
Extract the zip file to your C: drive, then add C:\mingw64\bin to PATH in system variables.

### Build ###
Run cmd as an administrator and go to the root directory of cpp-xpx-chain-sdk, then do the following:

```
mkdir build
cd build
cmake -G "MinGW Makefiles" ..
mingw32-make -j 6
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
