#======================================================================
# JuniperEngine
#======================================================================
cmake_minimum_required(VERSION 3.14)
project(JuniperEngine VERSION 0.1 LANGUAGES C CXX)

#======================================================================
# C++ 17 Support
#======================================================================
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#======================================================================
# Housekeeping
#======================================================================
if(UNIX AND NOT APPLE)
    set(LINUX TRUE)
endif()

set(VEND_DIR "${PROJECT_SOURCE_DIR}/juniper/vendor")

#======================================================================
# GLFW
#======================================================================
set(GLFW_DIR "${VEND_DIR}/glfw")
set(BUILD_SHARED_LIBS ON CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

include_directories(${GLFW_DIR}/include)

add_subdirectory(${GLFW_DIR})

list(APPEND VEND_LIBS glfw)

#======================================================================
# Vulkan
#======================================================================
find_package(Vulkan)

if(WIN32)
    if (NOT Vulkan_FOUND)
        find_library(Vulkan_LIBRARY
                     NAMES vulkan-1 vulkan
                     PATHS ${VEND_DIR}/vulkan/windows/lib)
        if (Vulkan_LIBRARY)
            set(Vulkan_FOUND ON)
            MESSAGE("Using vendored Vulkan library")
            include_directories(${VEND_DIR}/vulkan/include)
            MESSAGE("Including Vulkan headers from ${VEND_DIR}/vulkan/include")
        endif()
    endif()
elseif(LINUX)
    if (NOT Vulkan_FOUND)
        find_library(Vulkan_LIBRARY
                     NAMES vulkan
                     HINTS "$ENV{VULKAN_SDK}/lib" "${VEND_DIR}/vulkan/linux/lib"
                     REQUIRED)
        if (Vulkan_LIBRARY)
            set(Vulkan_FOUND ON)
            MESSAGE("Using vendored Vulkan library")
            include_directories(${VEND_DIR}/vulkan/include)
            MESSAGE("Including Vulkan headers from ${VEND_DIR}/vulkan/include")
        endif()
    endif()
endif(WIN32)

if (NOT Vulkan_FOUND)
	message(FATAL_ERROR "Failed to find Vulkan")
else()
	message(STATUS ${Vulkan_LIBRARY})
endif()

include_directories(${Vulkan_INCLUDE_DIRS})
List(APPEND VEND_LIBS ${Vulkan_LIBRARIES})

#======================================================================
# GLM
#======================================================================
set(GLM_DIR "${VEND_DIR}/glm")
include_directories(${VEND_DIR}/glm)

#======================================================================
# Juniper source code
#======================================================================

#======================================================================
# Juniper Entry Point
#======================================================================
add_executable(${PROJECT_NAME} juniper/main.cpp)

target_link_libraries(
  ${PROJECT_NAME}
  PRIVATE
  ${VEND_LIBS})
