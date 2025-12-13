cmake_minimum_required(VERSION 3.14)
project(Farkle LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL3 REQUIRED)
find_package(SDL3_ttf REQUIRED)

# Auto-download Roboto font into build/assets the first time
set(FONT_DIR ${CMAKE_CURRENT_BINARY_DIR}/assets)
file(MAKE_DIRECTORY ${FONT_DIR})
set(FONT_PATH ${FONT_DIR}/Roboto-Medium.ttf)
if(NOT EXISTS ${FONT_PATH})
    message(STATUS "Downloading Roboto font...")
    file(DOWNLOAD
        https://github.com/googlefonts/roboto/raw/main/src/hinted/Roboto-Medium.ttf
        ${FONT_PATH}
        STATUS download_status
    )
    list(GET download_status 0 status_code)
    if(NOT status_code EQUAL 0)
        message(FATAL_ERROR "Font download failed")
    endif()
endif()
add_executable(${PROJECT_NAME}
    src/main.cpp
    src/Text.cpp
    src/Dice.cpp
)
target_include_directories(${PROJECT_NAME} PRIVATE src)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3 SDL3_ttf::SDL3_ttf)