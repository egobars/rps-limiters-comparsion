add_library(workloads STATIC
    workload_base.cpp
    workload_base.h
)

target_include_directories(workloads PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(workload_static)
add_subdirectory(workload_parabolic)
add_subdirectory(workload_sinusoid)