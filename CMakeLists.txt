cmake_minimum_required(VERSION 3.19)
project(recordManager LANGUAGES C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(lockFile
    SHARED
        srcLockFile/lockfile.c
)
target_include_directories(lockFile PRIVATE include)

add_library(lockRegion
        SHARED
        srcLockRegion/lockRegion.c
)
target_include_directories(lockRegion PRIVATE include)


add_executable(recordManager
    include/os_defs.h
        include/os_libsymbols.h
        rm-db.c
        rm-lockwrapper.c
        rm-main.c
)
target_include_directories(recordManager PRIVATE include)

# if building with gcc and config is debug, compile with -g3
target_compile_options(recordManager PRIVATE $<$<AND:$<CONFIG:Debug>,$<OR:$<C_COMPILER_ID:GNU>,$<C_COMPILER_ID:Clang>>>:-g3 -Og>)

# address sanitizers for all platforms if compiled under debug
target_compile_options(recordManager PRIVATE $<$<AND:$<CONFIG:Debug>,$<OR:$<C_COMPILER_ID:GNU>,$<C_COMPILER_ID:Clang>,$<C_COMPILER_ID:AppleClang>>>:-fsanitize=address>)
target_link_options(recordManager PRIVATE $<$<AND:$<CONFIG:Debug>,$<OR:$<C_COMPILER_ID:GNU>,$<C_COMPILER_ID:Clang>,$<C_COMPILER_ID:AppleClang>>>:-fsanitize=address>)

target_compile_options(recordManager PRIVATE $<$<AND:$<CONFIG:Debug>,$<C_COMPILER_ID:MSVC>>:/fsanitize=address>)

