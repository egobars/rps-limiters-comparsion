cmake_minimum_required(VERSION 3.14)
project(RpsLimitersComparsion)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
    HTTPRequest
    GIT_REPOSITORY https://github.com/elnormous/HTTPRequest.git
    GIT_TAG master
)
FetchContent_MakeAvailable(HTTPRequest)

# Добавляем глобальный include директорий для всего проекта
include_directories(${httprequest_SOURCE_DIR}/include)

# Добавляем директории с алгоритмами, сервером, workloads и sender
add_subdirectory(algorithms)
add_subdirectory(server)
add_subdirectory(workloads)
add_subdirectory(sender)

add_executable(main main.cpp)

target_link_libraries(main PRIVATE server workloads workload_static workload_parabolic sender)
