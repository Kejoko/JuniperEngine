#======================================================================
# JuniperEngine
#======================================================================
cmake_minimum_required(VERSION 3.14)
project(Juniper VERSION 0.1 LANGUAGES CXX)

#======================================================================
# C++ 14 Support
#======================================================================
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#======================================================================
# GLFW
#======================================================================
set(BUILD_SHARED_LIBS ON CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

include_directories(
  ${PROJECT_SOURCE_DIR}/juniper/deps/glfw/include)

add_subdirectory(juniper/deps/glfw)

list(APPEND DEP_LIBS glfw)