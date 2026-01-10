cmake_minimum_required(VERSION 3.11)
project(raylib-project)

include(FetchContent)

FetchContent_Declare(
    raylib
    # DOWNLOAD THE ZIP DIRECTLY INSTEAD OF CLONING
    URL https://github.com/raysan5/raylib/archive/refs/tags/5.5.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

FetchContent_MakeAvailable(raylib)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE raylib)

# --- NEW COMMAND IS BELOW ---
# This copies the entire 'resources' folder into the build folder
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/resources" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
