add_library(workload_spikes STATIC
    workload_spikes.cpp
    workload_spikes.h
)

target_include_directories(workload_spikes PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(workload_spikes PUBLIC workloads) 