cmake_minimum_required(VERSION 3.10)
project(ConwayGame C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_BUILD_TYPE Debug)

# Add Homebrew SDL2 paths explicitly for macOS
set(SDL2_DIR "/opt/homebrew/lib/cmake/SDL2")
set(SDL2_INCLUDE_DIRS "/opt/homebrew/include")
set(SDL2_LIBRARIES "/opt/homebrew/lib/libSDL2.dylib")
set(SDL2_TTF_LIBRARIES "/opt/homebrew/lib/libSDL2_ttf.dylib")

include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR})

add_executable(conway 
    main.c 
    logic.c 
    sdl_utils.c
)

target_link_libraries(conway ${SDL2_LIBRARIES} ${SDL2_TTF_LIBRARIES})