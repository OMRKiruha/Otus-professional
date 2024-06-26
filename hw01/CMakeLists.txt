cmake_minimum_required(VERSION 3.10)

set(PATCH_VERSION "2" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

project(01_helloworld VERSION ${PROJECT_VESRION})

configure_file(version.h.in version.h)

add_executable(helloworld_cli main.cpp)
add_library(01_helloworld lib.cpp)

set_target_properties(helloworld_cli 01_helloworld PROPERTIES
        CXX_STANDARD 14
        CXX_STANDARD_REQUIRED ON
)
target_include_directories(01_helloworld
        PRIVATE "${CMAKE_BINARY_DIR}"
)

target_link_libraries(helloworld_cli PRIVATE
        01_helloworld
)

if (MSVC)
    target_compile_options(helloworld_cli PRIVATE
            /W4
    )
    target_compile_options(01_helloworld PRIVATE
            /W4
    )
else ()
    target_compile_options(helloworld_cli PRIVATE
            -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(01_helloworld PRIVATE
            -Wall -Wextra -pedantic -Werror
    )
endif ()


install(TARGETS helloworld_cli RUNTIME DESTINATION bin)


set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_NAME helloworld)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_SET_DESTDIR ${CMAKE_CURRENT_BINARY_DIR})

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)

enable_testing()

set(WITH_TESTS ON CACHE BOOL "" FORCE)
if (WITH_TESTS)
    find_package(googletest)
    if (NOT googletest_FOUND)
        message("${Yellow}==> !!! GoogleTest not found${ColourReset}")
        include(FetchContent)
        FetchContent_Declare(
                googletest
                GIT_REPOSITORY https://github.com/google/googletest.git
                GIT_TAG v1.14.0
        )
        FetchContent_MakeAvailable(googletest)
        message("${Green}==> !!! FetchContent GoogleTest from GIT${ColourReset}")
    endif ()

    # Test target
    set(test_DIR ${CMAKE_CURRENT_LIST_DIR}/tests/)
    add_executable(test_version
            test_version.cpp
    )

    # Link test target against gtest libraries
    target_link_libraries(test_version PRIVATE
            gtest
            #gmock
            gtest_main
            #gmock_main
            01_helloworld
    )
    set_target_properties(test_version PROPERTIES
            CXX_STANDARD 14
            CXX_STANDARD_REQUIRED ON
            GTEST_COLOR ON
    )
    target_compile_options(test_version PUBLIC
            -g
            -Wall
            -Wextra
            -pedantic
            #-fsanitize=leak
            #-fsanitize=address
            #-fsanitize=undefined
    )

    #set(COMM_SANITIZER "-fsanitize=address -fsanitize=undefined -fsanitize-recover=all")
    #set(CMAKE_CXX_FLAGS ${COMM_SANITIZER})
    #set(DCMAKE_C_FLAGS ${COMM_SANITIZER})
    #set(CMAKE_EXE_LINKER_FLAGS ${COMM_SANITIZER})
endif ()

add_test(test_version test_version)