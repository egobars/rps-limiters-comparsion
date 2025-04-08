add_library(worker STATIC worker.cpp worker.h)

find_package(PkgConfig REQUIRED)
pkg_check_modules(HTTPSERVER REQUIRED libhttpserver>=0.19)
pkg_check_modules(MICROHTTPD REQUIRED libmicrohttpd>=0.9.52)

target_link_libraries(worker PRIVATE 
    ${HTTPSERVER_LIBRARIES}
    ${MICROHTTPD_LIBRARIES}
    algorithms
)

target_include_directories(worker PRIVATE
    ${HTTPSERVER_INCLUDE_DIRS}
    ${MICROHTTPD_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}
)
