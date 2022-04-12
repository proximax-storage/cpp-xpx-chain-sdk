cmake_minimum_required(VERSION 3.9)
project(cpp-xpx-chain-sdk VERSION 1.0.0)
include(CMakeGlobalFunctions.cmake)

# CMP0068: fallback to older policies for macOS
cmake_policy(SET CMP0068 OLD)

#-----------------------------------------------------------------------------
# options and features
#-----------------------------------------------------------------------------

option(XPX_CHAIN_SDK_BUILD_STATIC "Build static version of XPX-CHAIN SDK library." OFF)
option(XPX_CHAIN_SDK_BUILD_TESTS "Build XPX-CHAIN SDK tests." ON)
option(XPX_CHAIN_SDK_BUILD_EXAMPLES "Build XPX-CHAIN SDK examples." ON)
option(XPX_CHAIN_SDK_ENABLE_WEBSOCKETS "Enable WebSockets API." OFF)

if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
	set(CMAKE_BUILD_TYPE "Release" CACHE STRING "XPX-CHAIN SDK build type." FORCE)
	set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
		"Debug" "Release" "RelWithDebInfo" "MinSizeRel")
endif()

if (MSVC)
	add_compile_options(/W3)
else()
	add_compile_options(-Wall)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(cmake/CheckFeatures.cmake)
check_string_literal_operator_template(HAVE_STRING_LITERAL_OPERATOR_TEMPLATE)

#-----------------------------------------------------------------------------
# dependencies
#-----------------------------------------------------------------------------
if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
	message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
	file(DOWNLOAD "https://github.com/conan-io/cmake-conan/raw/v0.14/conan.cmake"
		"${CMAKE_BINARY_DIR}/conan.cmake")
endif()

include(${CMAKE_BINARY_DIR}/conan.cmake)

conan_cmake_run(REQUIRES
		rapidjson/1.1.0@bincrafters/stable
		boost/1.71.0
		openssl/1.1.1k
	BASIC_SETUP
	CMAKE_TARGETS
	BUILD missing)

#-----------------------------------------------------------------------------
# sources
#-----------------------------------------------------------------------------

file(GLOB_RECURSE sources
	"include/xpxchaincpp/*.h"
	"src/infrastructure/*.h"
	"src/infrastructure/*.cpp"
	"src/sdk/*.cpp")

file(GLOB ref10_sources
	"src/third-party/ref10/*.c"
	"src/third-party/ref10/*.h")

file(GLOB ripemd160_sources
	"src/third-party/ripemd160/*.c"
	"src/third-party/ripemd160/*.h")

file(GLOB sha3_sources
	"src/third-party/sha3/*.c"
	"src/third-party/sha3/*.h")

file(GLOB_RECURSE sha256_sources
	"src/third-party/sha256/*.c"
	"src/third-party/sha256/*.h")

if (MSVC)
	# disable silly warning 'integral constant overflow' which is not easily fixed for 64bit types in constexpr
	set_source_files_properties(${sources} PROPERTIES
		COMPILE_FLAGS "/wd4307")

	# disable warnings from third-party sources
	set_source_files_properties(${ref10_sources} ${ripemd160_sources} ${sha3_sources} ${sha256_sources} PROPERTIES
		COMPILE_FLAGS "/wd4244 /wd4146")
else()
	# disable clang warning about using non-standard string literal operator template (N3599 proposal)
	if (HAVE_STRING_LITERAL_OPERATOR_TEMPLATE AND CMAKE_CXX_COMPILER_ID MATCHES "Clang")
		set_source_files_properties(${sources} PROPERTIES
			COMPILE_FLAGS "-Wno-gnu-string-literal-operator-template")
	endif()

	# disable all warnings from third-party sources
	set_source_files_properties(${ref10_sources} ${ripemd160_sources} ${sha3_sources} ${sha256_sources} PROPERTIES
		COMPILE_FLAGS "-w")
endif()

list(APPEND sources ${ref10_sources} ${ripemd160_sources} ${sha3_sources} ${sha256_sources})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${sources})

#-----------------------------------------------------------------------------
# targets
#-----------------------------------------------------------------------------

if (XPX_CHAIN_SDK_BUILD_STATIC)
	add_library(xpxchaincpp STATIC "")
	set_target_properties(xpxchaincpp PROPERTIES
		OUTPUT_NAME "xpxchaincpp-static")
else()

	add_library(xpxchaincpp SHARED "")

	set_target_properties(xpxchaincpp PROPERTIES
		VERSION ${PROJECT_VERSION}
		SOVERSION ${PROJECT_VERSION_MAJOR})

	if (MSVC)
		set_target_properties(xpxchaincpp PROPERTIES
			WINDOWS_EXPORT_ALL_SYMBOLS ON)
	endif()
endif()

include(GNUInstallDirs)

target_sources(xpxchaincpp PRIVATE ${sources})

target_compile_definitions(xpxchaincpp PRIVATE
	XPX_CHAIN_USE_STRING_LITERAL_OPERATOR_TEMPLATE=${HAVE_STRING_LITERAL_OPERATOR_TEMPLATE})

target_include_directories(xpxchaincpp
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/src)

if(XPX_CHAIN_SDK_ENABLE_WEBSOCKETS)
    target_compile_definitions(xpxchaincpp PRIVATE XPX_CHAIN_SDK_ENABLE_WEBSOCKETS)
endif()

target_link_libraries(xpxchaincpp
		CONAN_PKG::rapidjson
		CONAN_PKG::boost
		CONAN_PKG::openssl)

#-----------------------------------------------------------------------------
# install
#-----------------------------------------------------------------------------

include(CMakePackageConfigHelpers)

set(package_config_dir ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})
set(package_config_prefix ${PROJECT_NAME})

if (XPX_CHAIN_SDK_BUILD_STATIC)
	string(APPEND package_config_dir "-static")
	string(APPEND package_config_prefix "-static")
endif()

configure_package_config_file(
	cmake/cpp-xpx-chain-sdk-config.cmake.in
	"${CMAKE_CURRENT_BINARY_DIR}/${package_config_prefix}-config.cmake"
	INSTALL_DESTINATION "${package_config_dir}"
	NO_SET_AND_CHECK_MACRO
	NO_CHECK_REQUIRED_COMPONENTS_MACRO)

write_basic_package_version_file(${package_config_prefix}-config-version.cmake
	COMPATIBILITY SameMajorVersion)

install(TARGETS xpxchaincpp EXPORT ${package_config_prefix}-targets
	ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
	LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
	RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}")

install(DIRECTORY include/xpxchaincpp
	DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")

install(EXPORT ${package_config_prefix}-targets
	FILE ${package_config_prefix}-targets.cmake
	DESTINATION "${package_config_dir}"
	NAMESPACE XPX_CHAIN::)

install(FILES
	"${CMAKE_CURRENT_BINARY_DIR}/${package_config_prefix}-config.cmake"
	"${CMAKE_CURRENT_BINARY_DIR}/${package_config_prefix}-config-version.cmake"
	DESTINATION "${package_config_dir}")

#-----------------------------------------------------------------------------
# tests
#-----------------------------------------------------------------------------

if (XPX_CHAIN_SDK_BUILD_TESTS)
	enable_testing()
	add_subdirectory(tests)
endif()

if (XPX_CHAIN_SDK_BUILD_EXAMPLES)
	add_executable(get_block ${CMAKE_CURRENT_SOURCE_DIR}/examples/get_block.cpp)
	target_link_libraries(get_block
			xpxchaincpp
			CONAN_PKG::rapidjson
			CONAN_PKG::boost
			CONAN_PKG::openssl)

	add_executable(bonded_aggregate_transaction ${CMAKE_CURRENT_SOURCE_DIR}/examples/bonded_aggregate_transaction.cpp)
	target_link_libraries(bonded_aggregate_transaction
			xpxchaincpp
			CONAN_PKG::rapidjson
			CONAN_PKG::boost
			CONAN_PKG::openssl)

	add_executable(liquidity_provider_transactions ${CMAKE_CURRENT_SOURCE_DIR}/examples/liquidity_provider_transactions.cpp)
	target_link_libraries(liquidity_provider_transactions
			xpxchaincpp
			CONAN_PKG::rapidjson
			CONAN_PKG::boost
			CONAN_PKG::openssl)
endif()
