CMAKE_MINIMUM_REQUIRED (VERSION 3.2)
PROJECT (maestro)

option(BUILD_SHARED_LIBS "Build shared libraries" ON)
option(BUILD_MACOSX_BUNDLE "If it should build a .bundle library" OFF)

SET(MAESTRO_OUTPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/_builds)
SET(CONAN_DEPS_DIR ${MAESTRO_OUTPUT_DIR}/conan)

if(EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    conan_basic_setup(TARGETS NO_OUTPUT_DIRS)
elseif(EXISTS ${CONAN_DEPS_DIR}/conanbuildinfo.cmake)
    include(${CONAN_DEPS_DIR}/conanbuildinfo.cmake)
    conan_basic_setup(TARGETS NO_OUTPUT_DIRS)
endif()

SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory (cpplib)
add_subdirectory (room-example)
