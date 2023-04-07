cmake_minimum_required(VERSION 3.9)
project(cpp-xpx-chain-sdk VERSION 1.0.0)

#-----------------------------------------------------------------------------
# options and features
#-----------------------------------------------------------------------------

option(XPX_CHAIN_SDK_BUILD_STATIC "Build static version of XPX-CHAIN SDK library." OFF)
option(XPX_CHAIN_SDK_BUILD_TESTS "Build XPX-CHAIN SDK tests." OFF)
option(XPX_CHAIN_SDK_BUILD_EXAMPLES "Build XPX-CHAIN SDK examples." ON)
option(XPX_CHAIN_SDK_ENABLE_WEBSOCKETS "Enable WebSockets API." OFF)

include(CMakeGlobalFunctions.cmake)

if (APPLE)
	set(CMAKE_VERBOSE_MAKEFILE ON)
	set(BOOST_INCLUDE_DIR "/usr/local/include")
	set(BOOST_LIB_DIR "/usr/local/lib")
	set(OPENSSL_ROOT_DIR "/usr/local/opt/openssl")
	set(OPENSSL_LIBRARIES "/usr/local/opt/openssl/lib")
else()
endif()

find_package(Threads REQUIRED)
find_package(Boost 1.71.0 REQUIRED)
find_package(OpenSSL 1.1.1 REQUIRED)

if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
	set(CMAKE_BUILD_TYPE "Release" CACHE STRING "XPX-CHAIN SDK build type." FORCE)
	set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
		"Debug" "Release" "RelWithDebInfo" "MinSizeRel")
endif()

if (MSVC)
	add_compile_options(/W3)
	add_definitions(-D_WIN32_WINNT=0x0601)
else()
	add_compile_options(-w)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(cmake/CheckFeatures.cmake)
check_string_literal_operator_template(HAVE_STRING_LITERAL_OPERATOR_TEMPLATE)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
endif()

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
		include/rapidjson/include/
		${CMAKE_CURRENT_SOURCE_DIR}/src)

if(XPX_CHAIN_SDK_ENABLE_WEBSOCKETS)
    target_compile_definitions(xpxchaincpp PRIVATE XPX_CHAIN_SDK_ENABLE_WEBSOCKETS)
endif()

if(APPLE)
    include_directories(${BOOST_INCLUDE_DIR})
    include_directories(${OPENSSL_INCLUDE_DIR})
	target_include_directories(xpxchaincpp PUBLIC ${Boost_INCLUDE_DIRS})
else()
	include_directories(${Boost_INCLUDE_DIR})
endif()

target_link_libraries(xpxchaincpp
					  ${Boost_LIBRARIES}
					  OpenSSL::SSL
					  OpenSSL::Crypto
					  pthread)

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

if (APPLE)
else()

    if (XPX_CHAIN_SDK_BUILD_TESTS)
            enable_testing()
            add_subdirectory(tests)
    endif()

    if (XPX_CHAIN_SDK_BUILD_EXAMPLES)
            add_executable(get_block ${CMAKE_CURRENT_SOURCE_DIR}/examples/get_block.cpp)
            target_link_libraries(get_block
								xpxchaincpp
								${Boost_LIBRARIES}
								OpenSSL::SSL
            					OpenSSL::Crypto
								pthread)

            add_executable(bonded_aggregate_transaction ${CMAKE_CURRENT_SOURCE_DIR}/examples/bonded_aggregate_transaction.cpp)
            target_link_libraries(bonded_aggregate_transaction
								xpxchaincpp
								${Boost_LIBRARIES}
								OpenSSL::SSL
            					OpenSSL::Crypto
								pthread)

            add_executable(liquidity_provider_transactions ${CMAKE_CURRENT_SOURCE_DIR}/examples/liquidity_provider_transactions.cpp)
            target_link_libraries(liquidity_provider_transactions
								xpxchaincpp
							    ${Boost_LIBRARIES}
								OpenSSL::SSL
            					OpenSSL::Crypto
								pthread)
    endif()
endif()
