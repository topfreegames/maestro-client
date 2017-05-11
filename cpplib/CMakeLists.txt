FILE(GLOB example_SRC
    "src/*.cpp"
    "include/*.h"
)

FIND_LIBRARY(RC_LIB_LOCATION restclient-cpp)
IF(RC_LIB_LOCATION)
  MESSAGE(STATUS ${RC_LIB_LOCATION})
ELSE()
  MESSAGE(FATAL_ERROR "FATAL ERROR: missing library")
ENDIF()
ADD_LIBRARY(maestro-client SHARED ${example_SRC})
TARGET_LINK_LIBRARIES(maestro-client ${RC_LIB_LOCATION})
TARGET_COMPILE_FEATURES(maestro-client PRIVATE cxx_range_for)
