add_library(workload_parabolic STATIC
    workload_parabolic.cpp
    workload_parabolic.h
)

target_include_directories(workload_parabolic PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(workload_parabolic PUBLIC workloads) 