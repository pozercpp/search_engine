cmake_minimum_required(VERSION 3.16)
project(SearchEngine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

if(CMAKE_BUILD_TYPE MATCHES Release)
    if(MSVC)
        add_compile_options(/O2)
    else()
        add_compile_options(-O3 -march=native)
    endif()
endif()

find_package(nlohmann_json QUIET)

if(NOT nlohmann_json_FOUND)
    include(FetchContent)
    FetchContent_Declare(
        json
        URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
    )
    FetchContent_MakeAvailable(json)
endif()

file(GLOB_RECURSIVE SOURCES
    "src/*.cpp"
    "src/*/*.cpp"
    "main.cpp"
)

file(GLOB_RECURSIVE HEADERS
    "src/*.hpp"
    "src/*/*.hpp"
)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        nlohmann_json::nlohmann_json
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        src
        src/indexer
        src/index
        src/tokenizer
        src/query
        src/ranking
        src/utils
)

if(NOT MSVC)
    target_link_libraries(${PROJECT_NAME} PRIVATE stdc++fs)
endif()

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/data $<TARGET_FILE_DIR:${PROJECT_NAME}>/data
)

message(STATUS "Проект: ${PROJECT_NAME}")
message(STATUS "Режим сборки: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ стандарт: C++${CMAKE_CXX_STANDARD}")