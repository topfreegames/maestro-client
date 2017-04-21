ADD_EXECUTABLE(example ./src/example.cpp)
TARGET_COMPILE_FEATURES(example PRIVATE cxx_range_for)
TARGET_LINK_LIBRARIES(example restclient-cpp maestro-client)
