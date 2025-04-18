add_library(workload_sinusoid STATIC
    workload_sinusoid.cpp
    workload_sinusoid.h
)

target_include_directories(workload_sinusoid PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(workload_sinusoid PUBLIC workloads) 