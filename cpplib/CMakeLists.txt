cmake_minimum_required(VERSION 3.5)

FILE(GLOB example_SRC
    "src/*.cpp"
    "include/*.h"
)

find_package(Boost REQUIRED)

FIND_LIBRARY(RC_LIB_LOCATION restclient-cpp)
IF(RC_LIB_LOCATION)
  MESSAGE(STATUS ${RC_LIB_LOCATION})
ELSE()
  MESSAGE(FATAL_ERROR "FATAL ERROR: missing 'restclient-cpp' library")
ENDIF()

if(BUILD_MACOSX_BUNDLE)
    set(module_lib SHARED)
endif()

ADD_LIBRARY(maestro ${module_lib} ${example_SRC})

if(BUILD_MACOSX_BUNDLE)
    message(STATUS "Will build a Macosx bundle")
    set_target_properties(maestro
      PROPERTIES
        SUFFIX ".bundle"
        BUNDLE TRUE)
endif()

if(APPLE)
    target_include_directories(maestro
      PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
      PRIVATE
        ${CMAKE_SOURCE_DIR}/deps/restclient-cpp/mac/include)
elseif(UNIX)
    target_include_directories(maestro PRIVATE ${CMAKE_SOURCE_DIR}/deps/restclient-cpp/linux/include)
    set_target_properties(maestro PROPERTIES POSITION_INDEPENDENT_CODE ON)
endif()

if(TARGET CONAN_PKG::libcurl)
    message(STATUS "libcurl found with conan")
    target_link_libraries(maestro PRIVATE CONAN_PKG::libcurl)
else()
    message(STATUS "libcurl will be searched in the system")
    find_package(CURL REQUIRED)
    target_link_libraries(maestro PRIVATE CURL::libcurl)
endif()

target_link_libraries(maestro
  PRIVATE
    ${RC_LIB_LOCATION}
    Boost::boost)

TARGET_COMPILE_FEATURES(maestro PRIVATE cxx_range_for)
