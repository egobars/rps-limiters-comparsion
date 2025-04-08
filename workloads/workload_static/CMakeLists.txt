add_library(workload_static STATIC
    workload_static.cpp
    workload_static.h
)

target_include_directories(workload_static PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(workload_static PUBLIC workloads) 