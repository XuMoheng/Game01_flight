cmake_minimum_required(VERSION 4.2.3)
project(SDLShooter VERSION 0.1.0 LANGUAGES C CXX)

# C++ Settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Output Settings
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin)

# SDL2 Settings
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_ttf REQUIRED)

set(TARGET ${PROJECT_NAME}-${CMAKE_SYSTEM_NAME})
add_executable(${TARGET})
target_sources(${TARGET}
    PRIVATE
        src/main.cpp
        src/Game.cpp
        src/SceneMain.cpp
)

target_link_libraries(${TARGET}
    SDL2::SDL2
    SDL2_image::SDL2_image
    SDL2_mixer::SDL2_mixer
    SDL2_ttf::SDL2_ttf)
