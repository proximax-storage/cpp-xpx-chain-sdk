# Method to get GTest and link it from https://google.github.io/googletest/quickstart-cmake.html

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

function(add_single_test TARGET_NAME SOURCE_CPP)
    enable_testing()

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}" PARENT_SCOPE)

    # test libraries are in the form test.xyz, so add xyz as a dependency (the library under test)
    add_executable(${TARGET_NAME} ${SOURCE_CPP})

    target_link_libraries(${TARGET_NAME} gtest gtest_main)
    target_link_libraries(${TARGET_NAME}
            xpxchaincpp
            CONAN_PKG::rapidjson
            CONAN_PKG::boost
            CONAN_PKG::openssl)

    include(GoogleTest)
    gtest_discover_tests(${TARGET_NAME})
endfunction()