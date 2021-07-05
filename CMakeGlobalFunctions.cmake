function(add_single_test TARGET_NAME SOURCE_CPP)
    find_package(GTest REQUIRED)
    include_directories(SYSTEM ${GTEST_INCLUDE_DIR})

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}" PARENT_SCOPE)

    # test libraries are in the form test.xyz, so add xyz as a dependency (the library under test)
    add_executable(${TARGET_NAME} ${SOURCE_CPP})
    add_test(
            NAME ${TARGET_NAME}
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            COMMAND ${TARGET_NAME}
    )

    target_link_libraries(${TARGET_NAME} gtest gtest_main)
    target_link_libraries(${TARGET_NAME}
            xpxchaincpp
            CONAN_PKG::rapidjson
            CONAN_PKG::boost
            CONAN_PKG::openssl)
endfunction()