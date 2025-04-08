add_library(algorithm_mock STATIC algorithm_mock.cpp algorithm_mock.h)

target_include_directories(algorithm_mock PUBLIC ${CMAKE_SOURCE_DIR})
