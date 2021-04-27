#======================================================================
# JuniperEngine
#======================================================================
cmake_minimum_required(VERSION 3.11)
set(CMAKE_CONFIGURATION_TYPES
    Debug Test Release
    CACHE STRING "" FORCE)
project(JuniperEngine VERSION 0.1 LANGUAGES C CXX)

#======================================================================
# C++ 17 Support
#======================================================================
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#======================================================================
# Housekeeping
#======================================================================
if(NOT CMAKE_BUILD_TYPE)
    message(STATUS "No build type specified. Defaulting to Debug")
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "" FORCE)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Building in Debug mode. Suppressing no messages")
    add_compile_definitions(BUILD_DEBUG)
elseif(CMAKE_BUILD_TYPE STREQUAL "Test")
    message(STATUS "Building in Test mode. Suppressing debug messages")
    add_compile_definitions(BUILD_TEST)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    message(STATUS "Building in Release mode. Suppressing non-error messages")
    add_compile_definitions(BUILD_RELEASE)
else()
    message(FATAL_ERROR "Unkown build type specified: ${CMAKE_BUILD_TYPE}")
endif()

if(UNIX AND NOT APPLE)
    set(LINUX TRUE)
endif()

set(SRC_DIR "${PROJECT_SOURCE_DIR}/juniper/src")
set(VEND_DIR "${PROJECT_SOURCE_DIR}/juniper/vendor")

#======================================================================
# GLFW
#======================================================================
set(GLFW_DIR "${VEND_DIR}/glfw")

set(INCL_GLFW_STR "Including GLFW headers from:")

if(WIN32)
    message(STATUS "Using precompiled GLFW binaries")
    find_library(Glfw_LIBRARY
             NAMES glfw3
             PATHS ${GLFW_DIR}/windows/lib)
    if(Glfw_LIBRARY)
        set(Glfw_FOUND ON)
        message(STATUS "Found precompiled GLFW libraries: ${Glfw_LIBRARY}")
        message(STATUS "${INCL_GLFW_STR} ${GLFW_DIR}/windows/include")
        include_directories(${GLFW_DIR}/windows/include)
        list(APPEND VEND_LIBS ${Glfw_LIBRARY})
    else()
        message(FATAL_ERROR "Failed to find GLFW binaries")
    endif()
else()
    message(STATUS "Building vendored GLFW from source")
    set(BUILD_SHARED_LIBS ON CACHE BOOL "" FORCE)
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    add_subdirectory(${GLFW_DIR})
    message(STATUS "${INCL_GLFW_STR} ${GLFW_DIR}/include")
    include_directories(${GLFW_DIR}/include)
    list(APPEND VEND_LIBS glfw)
endif()

#======================================================================
# Vulkan
#======================================================================
message(STATUS "Finding Vulkan")
find_package(Vulkan)

set(FAIL_VK_STR "Failed to find Vulkan with CMake module. Searching elsewhere")
set(VEND_VK_STR "Using vendored Vulkan library")
set(INCL_VK_STR "Including Vulkan headers from:")
set(LINK_VK_STR "Linking Vulkan libraries from:")

if(WIN32)
    if(NOT Vulkan_FOUND)
        message(STATUS "${FAIL_VK_STR}")
        find_library(Vulkan_LIBRARY
                     NAMES vulkan-1 vulkan
                     PATHS ${VEND_DIR}/vulkan/windows/lib)
        if(Vulkan_LIBRARY)
            set(Vulkan_FOUND ON)
            message(STATUS "${VEND_VK_STR}")
            message(STATUS "${INCL_VK_STR} ${VEND_DIR}/vulkan/include")
            include_directories(${VEND_DIR}/vulkan/include)
            message(STATUS "${LINK_VK_STR} ${Vulkan_LIBRARY}")
            List(APPEND VEND_LIBS ${Vulkan_LIBRARY})
        endif()
    else()
	    message(STATUS "${INCL_VK_STR} ${Vulkan_INCLUDE_DIRS}")
	    include_directories(${Vulkan_INCLUDE_DIRS})
        message(STATUS "${LINK_VK_STR} ${Vulkan_LIBRARIES}")
        List(APPEND VEND_LIBS ${Vulkan_LIBRARIES})
    endif()
elseif(LINUX)
    if(NOT Vulkan_FOUND)
        message(STATUS "${FAIL_VK_STR}")
        find_library(Vulkan_LIBRARY
                     NAMES vulkan
                     HINTS "$ENV{VULKAN_SDK}/lib" "${VEND_DIR}/vulkan/linux/lib"
                     REQUIRED)
        if(Vulkan_LIBRARY)
            set(Vulkan_FOUND ON)
            message(STATUS "${VEND_VK_STR}")
            message(STATUS "${INCL_VK_STR} ${VEND_DIR}/vulkan/include")
            include_directories(${VEND_DIR}/vulkan/include)
            message(STATUS "${LINK_VK_STR} ${Vulkan_LIBRARY}")
            List(APPEND VEND_LIBS ${Vulkan_LIBRARY})
        endif()
    else()
	    message(STATUS "${INCL_VK_STR} ${Vulkan_INCLUDE_DIRS}")
	    include_directories(${Vulkan_INCLUDE_DIRS})
        message(STATUS "${LINK_VK_STR} ${Vulkan_LIBRARIES}")
        List(APPEND VEND_LIBS ${Vulkan_LIBRARIES})
    endif()
elseif(APPLE)
    if(NOT Vulkan_FOUND)
        message(STATUS "${FAIL_VK_STR}")
        find_library(Vulkan_LIBRARY
                     NAMES vulkan
                     HINTS "$ENV{VULKAN_SDK}/lib" "${VEND_DIR}/vulkan/macOS/lib"
                     REQUIRED)
        if(Vulkan_LIBRARY)
            set(Vulkan_FOUND ON)
            message(STATUS "${VEND_VK_STR}")
            include_directories(${VEND_DIR}/vulkan/include)
            message(STATUS "${INCL_VK_STR} ${VEND_DIR}/vulkan/include")
            message(STATUS "${LINK_VK_STR} ${Vulkan_LIBRARY}")
            List(APPEND VEND_LIBS ${Vulkan_LIBRARY})
        endif()
    else()
	    message(STATUS "${INCL_VK_STR} ${Vulkan_INCLUDE_DIRS}")
	    include_directories(${Vulkan_INCLUDE_DIRS})
        message(STATUS "${LINK_VK_STR} ${Vulkan_LIBRARIES}")
        List(APPEND VEND_LIBS ${Vulkan_LIBRARIES})
    endif()
endif()

if(NOT Vulkan_FOUND)
	message(FATAL_ERROR "Failed to find Vulkan")
else()
	message(STATUS "Found Vulkan library: ${Vulkan_LIBRARY}")
endif()

#======================================================================
# GLM
#======================================================================
set(GLM_DIR "${VEND_DIR}/glm")
include_directories(${VEND_DIR}/glm)

#======================================================================
# Spdlog
#======================================================================
set(SPDLOG_DIR "${VEND_DIR}/spdlog")

message(STATUS "Building vendored Spdlog from source")
add_subdirectory(${SPDLOG_DIR})

message(STATUS "Including Spdlog headers from: ${SPDLOG_DIR}/include")
include_directories(${SPDLOG_DIR}/include)

list(APPEND VEND_LIBS spdlog)

#======================================================================
# Juniper source code
#======================================================================
include_directories(${SRC_DIR})

add_library(jun_Logger
            STATIC
            ${SRC_DIR}/logger/Logger.cpp
            ${SRC_DIR}/logger/Logger.h)
list(APPEND JUN_LIBS jun_Logger)

#======================================================================
# Juniper Entry Point
#======================================================================
add_executable(${PROJECT_NAME} juniper/main.cpp)

target_link_libraries(
  ${PROJECT_NAME}
  PRIVATE
  ${JUN_LIBS}
  ${VEND_LIBS})
