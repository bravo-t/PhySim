cmake_minimum_required(VERSION 3.1)

project(PhySim)

SET(CMAKE_C_FLAGS "-O3 -march=native")
SET(CMAKE_C_FLAGS_DEBUG "-O0 -g -ggdb")
SET(CMAKE_C_FLAGS_RELEASE "-O3 -march=native")
SET(CMAKE_C_FLAGS_PROF "-O0 -g -pg")
SET(CMAKE_CXX_FLAGS "-O3 -march=native")
SET(CMAKE_CXX_FLAGS_DEBUG "-O0 -g -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native")
SET(CMAKE_CXX_FLAGS_PROF "-O0 -g -pg")

#message(PROJECT_SOURCE_DIR="${PROJECT_SOURCE_DIR}")
#message(PROJECT_CURRENT_LIST_DIR="${PROJECT_CURRENT_LIST_DIR}")
#message(PROJECT_CURRENT_SOURCE_DIR="${PROJECT_CURRENT_SOURCE_DIR}")
#message(CMAKE_SOURCE_DIR="${CMAKE_SOURCE_DIR}")

#list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/gui/raylib/cmake")

add_subdirectory(gui)

add_subdirectory(nbody)
