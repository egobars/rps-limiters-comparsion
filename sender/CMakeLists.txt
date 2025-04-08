add_library(sender STATIC
    sender.cpp
    sender.h
)

target_include_directories(sender PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(sender PUBLIC workloads) 