cmake_minimum_required(VERSION 3.10)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

project(ip_filter VERSION ${PROJECT_VESRION})

configure_file(version.h.in version.h)

add_executable(${PROJECT_NAME}
        main.cpp
        ip_filter.cpp
        ip_filter.h
)
set(CMAKE_EXE_LINKER_FLAGS "-static-libstdc++ -static-libgcc")

set_target_properties(${PROJECT_NAME} PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
)

target_link_libraries(${PROJECT_NAME} pthread)

if (MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
            /W4
    )
else ()
    target_compile_options(${PROJECT_NAME} PRIVATE
            -Wall
            -Wextra
            -pedantic
            #-Werror
    )
endif ()


install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)


set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_NAME ${PROJECT_NAME})

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_SET_DESTDIR ${CMAKE_CURRENT_BINARY_DIR})

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)

enable_testing()

set(WITH_TESTS OFF CACHE BOOL "" FORCE)
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