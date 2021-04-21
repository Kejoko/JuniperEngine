#======================================================================
# JuniperEngine
#======================================================================
cmake_minimum_required(VERSION 3.14)
project(JuniperEngine VERSION 0.1 LANGUAGES C CXX)

#======================================================================
# C++ 14 Support
#======================================================================
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#======================================================================
# Housekeeping
#======================================================================
set(DEPS_DIR "${PROJECT_SOURCE_DIR}/juniper/deps")

#======================================================================
# GLFW
#======================================================================
set(GLFW_DIR "${DEPS_DIR}/glfw")
set(BUILD_SHARED_LIBS ON CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

include_directories(${GLFW_DIR}/include)

add_subdirectory(juniper/deps/glfw)

list(APPEND DEP_LIBS glfw)

#======================================================================
# GLAD
#======================================================================
set(GLAD_DIR "${DEPS_DIR}/glad")

include_directories(${GLAD_DIR}/include)

add_library("glad" "${GLAD_DIR}/src/glad.c")

list(APPEND DEP_LIBS glad)

#======================================================================
# Juniper source code
#======================================================================

#======================================================================
# Juniper Entry Point
#======================================================================
add_executable(${PROJECT_NAME} juniper/testgl.cpp)

target_link_libraries(
  ${PROJECT_NAME}
  PRIVATE
  ${DEP_LIBS})