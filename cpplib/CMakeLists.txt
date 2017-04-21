SET(BUILD_SHARED_LIBS OFF)
FILE(GLOB example_SRC
    "src/*.cpp"
    "include/*.h"
)
ADD_LIBRARY(maestro-client STATIC ${example_SRC})
TARGET_COMPILE_FEATURES(maestro-client PRIVATE cxx_range_for)
