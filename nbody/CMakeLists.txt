cmake_minimum_required(VERSION 3.11) # FetchContent is available in 3.11+
project(nbody)

#message(PROJECT_SOURCE_DIR="${PROJECT_SOURCE_DIR}")
#message(PROJECT_CURRENT_LIST_DIR="${PROJECT_CURRENT_LIST_DIR}")
#message(PROJECT_CURRENT_SOURCE_DIR="${PROJECT_CURRENT_SOURCE_DIR}")
#message(CMAKE_SOURCE_DIR="${CMAKE_SOURCE_DIR}")
#message(CMAKE_CURRENT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}")
#message(CMAKE_CURRENT_BINARY_DIR="${CMAKE_CURRENT_BINARY_DIR}")


# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Dependencies
file(COPY ${CMAKE_CURRENT_BINARY_DIR}/../gui/raylib/raylib/raylib-config-version.cmake
          DESTINATION "${PROJECT_SOURCE_DIR}/../gui/raylib/cmake")
set(raylib_USE_STATIC_LIBS ON)
set(raylib_DIR "${PROJECT_SOURCE_DIR}/../gui/raylib/cmake")
set(raylib_VERBOSE 1)
find_package(raylib "5.0.0" REQUIRED) # QUIET or REQUIRED

# Our Project

file (GLOB srcs *.cpp)
file (GLOB headers *.h)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR}/../gui/raylib/raylib/include)

add_library(src_obj OBJECT ${srcs} ${headers})
FOREACH(item ${srcs})
  IF(${item} MATCHES "main.cpp")
    LIST(REMOVE_ITEM srcs ${item})
  ENDIF(${item} MATCHES "main.cpp")
ENDFOREACH(item)
add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} raylib)

# Checks if OSX and links appropriate frameworks (Only required on MacOS)
if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif()
