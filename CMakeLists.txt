cmake_minimum_required(VERSION 3.19)
project(disneyplus)
include(ExternalProject)

set(CMAKE_CXX_STANDARD 14)
set(JSON_BuildTests OFF CACHE INTERNAL "")
#set(SDL2_CONFIG "cmake-build-debug/SDL2-2.0.14/sdl2-config")
add_compile_definitions(LOAD_JPG)
add_subdirectory(${PROJECT_SOURCE_DIR}/json)
add_subdirectory(${PROJECT_SOURCE_DIR}/SDL2-2.0.14/)
add_subdirectory(${PROJECT_SOURCE_DIR}/curl-7.76.1)
include_directories(SDL2-2.0.14/include json/single_include/nlohmann include SDL2_image-2.0.5)

add_executable(disneyplus main.cpp include/stb_image.h src/DisneyImage.cpp src/DisneyCurl.cpp src/DisneySet.cpp src/DisneySet.h)

target_link_libraries(disneyplus PRIVATE nlohmann_json::nlohmann_json SDL2 libcurl)