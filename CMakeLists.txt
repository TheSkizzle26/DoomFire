cmake_minimum_required(VERSION 3.31)
project(DoomFire C)

set(CMAKE_C_STANDARD 23)


include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
set(BUILD_GAMES    OFF CACHE BOOL "" FORCE) # don't build the supplied example games
FetchContent_Declare(
        raylib
        GIT_REPOSITORY "https://github.com/raysan5/raylib.git"
        GIT_TAG "master"
        GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(raylib)
list(APPEND LIBRARIES ${dep})


add_executable(DoomFire main.c)
target_link_libraries(DoomFire PRIVATE raylib)
