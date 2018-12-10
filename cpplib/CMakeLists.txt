FILE(GLOB example_SRC
    "src/*.cpp"
    "include/*.h"
)

find_package(Boost REQUIRED)

FIND_LIBRARY(RC_LIB_LOCATION restclient-cpp)
IF(RC_LIB_LOCATION)
  MESSAGE(STATUS ${RC_LIB_LOCATION})
ELSE()
  MESSAGE(FATAL_ERROR "FATAL ERROR: missing library")
ENDIF()

ADD_LIBRARY(maestro SHARED ${example_SRC})

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

TARGET_LINK_LIBRARIES(maestro PRIVATE ${RC_LIB_LOCATION} CONAN_PKG::libcurl Boost::boost)

TARGET_COMPILE_FEATURES(maestro PRIVATE cxx_range_for)
