cmake_minimum_required(VERSION 3.16)

set(PROJECT_NAME        simplefs)
set(PROJECT_VERSION     0.0.1)
project(${PROJECT_NAME} C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE "Debug")

if (NOT (CMAKE_COMPILER_IS_GNUCC AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 10.2.0) )
    message(FATAL_ERROR "You have to use GCC (ver >= 10.2.0) as the compiler!")
endif()

enable_testing()

include(modules/compile_definition.cmake)
include(modules/quick_add_test.cmake)
include(modules/add_utils.cmake)

include(FindPkgConfig)
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBFUSE REQUIRED fuse)
pkg_check_modules(LIBELF  REQUIRED libelf)
pkg_check_modules(LIBOPENSSL REQUIRED openssl)

add_compile_definitions("_FILE_OFFSET_BITS=64")
add_compile_definitions("PACKAGE_NAME=\"${PROJECT_NAME}\"")
add_compile_definitions("PACKAGE_VERSION=\"${PROJECT_VERSION}\"")

# enable code coverage
add_compile_options(-fprofile-arcs -ftest-coverage -O0 --coverage)
add_link_options(--coverage)

add_library(simplefs STATIC
        src/include/debug.h
        src/debug/elf_process.cpp
        src/debug/invoke_addr2line.cpp
        src/debug/simple_err.cpp    src/include/simple_err.h
        src/fs/blkmem.cpp           src/include/blkmem.h
        src/fs/io.cpp               src/include/io.h
        src/fs/simplefs.cpp         src/include/simplefs.h
        src/fs/utils.cpp            src/include/utils.h
        src/fs/bitmap_ctl.cpp       src/include/bitmap_ctl.h
        src/debug/log.cpp           src/include/log.h
        )
target_include_directories(simplefs PUBLIC src/include)
target_link_libraries(simplefs      PUBLIC ${LIBELF_LIBRARIES})
target_link_libraries(simplefs      PUBLIC ${LIBOPENSSL_LIBRARIES})
target_link_libraries(simplefs      PUBLIC pthread)
target_link_libraries(simplefs      PUBLIC gcov)

target_include_directories(simplefs PUBLIC ${LIBELF_INCLUDE_DIRS})
target_compile_options(simplefs     PUBLIC ${LIBELF_CFLAGS_OTHER})

target_include_directories(simplefs PUBLIC ${LIBOPENSSL_INCLUDE_DIRS})
target_compile_options(simplefs     PUBLIC ${LIBOPENSSL_CFLAGS_OTHER})

add_utils(mkfs.simplefs)
add_utils(fsck.simplefs)

_add_test(ftrace    "function trace system")
_add_test(blkmem    "block cache system")
_add_test(bitmap    "bitmap test")
_add_test(log       "test for runtime log")
