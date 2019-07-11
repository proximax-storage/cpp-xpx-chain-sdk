cmake_minimum_required(VERSION 3.8)
project(nem2-sdk-cpp VERSION 1.0.0)

#-----------------------------------------------------------------------------
# options and features
#-----------------------------------------------------------------------------

option(NEM2_SDK_BUILD_STATIC "Build static version of NEM2 SDK library." OFF)
option(NEM2_SDK_BUILD_TESTS "Build NEM2 SDK tests." OFF)

if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
	set(CMAKE_BUILD_TYPE "Release" CACHE STRING "NEM2 SDK build type." FORCE)
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
set(rapidjson_dir ${CMAKE_BINARY_DIR}/download/rapidjson)
configure_file(cmake/CMakeLists-rapidjson.txt.in "${rapidjson_dir}/CMakeLists.txt")

execute_process(COMMAND "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" .
	WORKING_DIRECTORY "${rapidjson_dir}")
execute_process(COMMAND "${CMAKE_COMMAND}" --build .
	WORKING_DIRECTORY "${rapidjson_dir}")

#-----------------------------------------------------------------------------
# sources
#-----------------------------------------------------------------------------

set(sources
	include/nemcpp/config.h
	include/nemcpp/exceptions.h
	include/nemcpp/sdk.h
	include/nemcpp/types.h
	include/nemcpp/crypto/hash.h
	include/nemcpp/crypto/key_pair.h
	include/nemcpp/crypto/private_key.h
	include/nemcpp/crypto/secure_zero.h
	include/nemcpp/crypto/signer.h
	include/nemcpp/model/account/account.h
	include/nemcpp/model/account/account_property.h
	include/nemcpp/model/account/address.h
	include/nemcpp/model/account/public_account.h
	include/nemcpp/model/mosaic/mosaic.h
	include/nemcpp/model/mosaic/mosaic_property.h
	include/nemcpp/model/namespace/namespace.h
	include/nemcpp/model/namespace/namespace_path.h
	include/nemcpp/model/transaction/account_link_transaction.h
	include/nemcpp/model/transaction/account_link_transaction_types.h
	include/nemcpp/model/transaction/account_property_transaction.h
	include/nemcpp/model/transaction/account_property_transaction_types.h
	include/nemcpp/model/transaction/address_alias_transaction.h
	include/nemcpp/model/transaction/aggregate_transaction.h
	include/nemcpp/model/transaction/aggregate_transaction_types.h
	include/nemcpp/model/transaction/alias_transaction_base.h
	include/nemcpp/model/transaction/alias_transaction_types.h
	include/nemcpp/model/transaction/basic_transaction.h
	include/nemcpp/model/transaction/embedded_transaction.h
	include/nemcpp/model/transaction/lock_funds_transaction.h
	include/nemcpp/model/transaction/modify_multisig_account_transaction.h
	include/nemcpp/model/transaction/modify_multisig_account_transaction_types.h
	include/nemcpp/model/transaction/mosaic_alias_transaction.h
	include/nemcpp/model/transaction/mosaic_definition_transaction.h
	include/nemcpp/model/transaction/mosaic_supply_change_transaction.h
	include/nemcpp/model/transaction/mosaic_supply_change_transaction_types.h
	include/nemcpp/model/transaction/register_namespace_transaction.h
	include/nemcpp/model/transaction/secret_hash_algorithm.h
	include/nemcpp/model/transaction/secret_lock_transaction.h
	include/nemcpp/model/transaction/secret_proof_transaction.h
	include/nemcpp/model/transaction/transaction.h
	include/nemcpp/model/transaction/transaction_signing.h
	include/nemcpp/model/transaction/transaction_type.h
	include/nemcpp/model/transaction/transfer_transaction.h
	include/nemcpp/service/read_result.h
	include/nemcpp/service/serialization.h
	include/nemcpp/utils/base32.h
	include/nemcpp/utils/bitwise_enum.h
	include/nemcpp/utils/buffer.h
	include/nemcpp/utils/comparator.h
	include/nemcpp/utils/format.h
	include/nemcpp/utils/hashable_array.h
	include/nemcpp/utils/hasher.h
	include/nemcpp/utils/lazy.h
	include/nemcpp/utils/network_time.h
	include/nemcpp/utils/noncopyable.h
	src/infrastructure/binary/descriptors.h
	src/infrastructure/binary/offsets.h
	src/infrastructure/binary/parser.cpp
	src/infrastructure/binary/parser.h
	src/infrastructure/binary/dto/transaction_dto.h
	src/infrastructure/json/descriptors.h
	src/infrastructure/json/hex.h
	src/infrastructure/json/parser.cpp
	src/infrastructure/json/parser.h
	src/infrastructure/json/uint64.cpp
	src/infrastructure/json/uint64.h
	src/infrastructure/json/dto/mosaic_dto.h
	src/infrastructure/utils/byte_stream.cpp
	src/infrastructure/utils/byte_stream.h
	src/infrastructure/utils/variadic_struct.h
	src/sdk/config.cpp
	src/sdk/crypto/hash.cpp
	src/sdk/crypto/key_pair.cpp
	src/sdk/crypto/private_key.cpp
	src/sdk/crypto/secure_zero.cpp
	src/sdk/crypto/signer.cpp
	src/sdk/model/account/account.cpp
	src/sdk/model/account/address.cpp
	src/sdk/model/account/public_account.cpp
	src/sdk/model/mosaic/mosaic.cpp
	src/sdk/model/mosaic/mosaic_property.cpp
	src/sdk/model/namespace/namespace.cpp
	src/sdk/model/namespace/namespace_path.cpp
	src/sdk/model/transaction/account_link_transaction.cpp
	src/sdk/model/transaction/account_property_transaction.cpp
	src/sdk/model/transaction/address_alias_transaction.cpp
	src/sdk/model/transaction/aggregate_transaction.cpp
	src/sdk/model/transaction/alias_transaction_base.cpp
	src/sdk/model/transaction/basic_transaction.cpp
	src/sdk/model/transaction/create_transaction.cpp
	src/sdk/model/transaction/create_transaction.h
	src/sdk/model/transaction/embedded_transaction.cpp
	src/sdk/model/transaction/lock_funds_transaction.cpp
	src/sdk/model/transaction/modify_multisig_account_transaction.cpp
	src/sdk/model/transaction/mosaic_alias_transaction.cpp
	src/sdk/model/transaction/mosaic_definition_transaction.cpp
	src/sdk/model/transaction/mosaic_supply_change_transaction.cpp
	src/sdk/model/transaction/register_namespace_transaction.cpp
	src/sdk/model/transaction/secret_hash_algorithm.cpp
	src/sdk/model/transaction/secret_lock_transaction.cpp
	src/sdk/model/transaction/secret_proof_transaction.cpp
	src/sdk/model/transaction/transaction.cpp
	src/sdk/model/transaction/transaction_signing.cpp
	src/sdk/model/transaction/transaction_type.cpp
	src/sdk/model/transaction/transaction_utils.cpp
	src/sdk/model/transaction/transaction_utils.h
	src/sdk/model/transaction/transfer_transaction.cpp
	src/sdk/service/binary_serialization.cpp
	src/sdk/service/read_result.cpp
	src/sdk/utils/base32.cpp
	src/sdk/utils/format.cpp
	src/sdk/utils/network_time.cpp)

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

if (NEM2_SDK_BUILD_STATIC)
	add_library(nemcpp STATIC "")
	set_target_properties(nemcpp PROPERTIES
		OUTPUT_NAME "nemcpp-static")
else()
	add_library(nemcpp SHARED "")
	set_target_properties(nemcpp PROPERTIES
		VERSION ${PROJECT_VERSION}
		SOVERSION ${PROJECT_VERSION_MAJOR})

	if (MSVC)
		set_target_properties(nemcpp PROPERTIES
			WINDOWS_EXPORT_ALL_SYMBOLS ON)
	endif()
endif()

include(GNUInstallDirs)

target_sources(nemcpp PRIVATE ${sources})

target_compile_definitions(nemcpp PRIVATE
	NEM2_USE_STRING_LITERAL_OPERATOR_TEMPLATE=${HAVE_STRING_LITERAL_OPERATOR_TEMPLATE})

target_include_directories(nemcpp
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/src
		"${rapidjson_dir}/src/include")

# adding alias for target (required to consistent use of SDK when
# it is built as subproject)
add_library("NEM2::nemcpp" ALIAS nemcpp)

#-----------------------------------------------------------------------------
# install
#-----------------------------------------------------------------------------

include(CMakePackageConfigHelpers)

set(package_config_dir ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})
set(package_config_prefix ${PROJECT_NAME})

if (NEM2_SDK_BUILD_STATIC)
	string(APPEND package_config_dir "-static")
	string(APPEND package_config_prefix "-static")
endif()

configure_package_config_file(
	cmake/nem2-sdk-cpp-config.cmake.in
	"${CMAKE_CURRENT_BINARY_DIR}/${package_config_prefix}-config.cmake"
	INSTALL_DESTINATION "${package_config_dir}"
	NO_SET_AND_CHECK_MACRO
	NO_CHECK_REQUIRED_COMPONENTS_MACRO)

write_basic_package_version_file(${package_config_prefix}-config-version.cmake
	COMPATIBILITY SameMajorVersion)

install(TARGETS nemcpp EXPORT ${package_config_prefix}-targets
	ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
	LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
	RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}")

install(DIRECTORY include/nemcpp
	DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")

install(EXPORT ${package_config_prefix}-targets
	FILE ${package_config_prefix}-targets.cmake
	DESTINATION "${package_config_dir}"
	NAMESPACE NEM2::)

install(FILES
	"${CMAKE_CURRENT_BINARY_DIR}/${package_config_prefix}-config.cmake"
	"${CMAKE_CURRENT_BINARY_DIR}/${package_config_prefix}-config-version.cmake"
	DESTINATION "${package_config_dir}")

#-----------------------------------------------------------------------------
# tests
#-----------------------------------------------------------------------------

if (NEM2_SDK_BUILD_TESTS)
	include(CTest)
	enable_testing()
	add_subdirectory(tests)
endif()
