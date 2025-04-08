add_library(algorithms INTERFACE)

add_subdirectory(algorithm_mock)

target_include_directories(algorithms INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(algorithms INTERFACE algorithm_mock)
