add_library(server STATIC server.cpp server.h)

find_package(PkgConfig REQUIRED)

pkg_check_modules(HTTPSERVER REQUIRED libhttpserver>=0.19)
pkg_check_modules(MICROHTTPD REQUIRED libmicrohttpd>=0.9.52)

find_package(CURL REQUIRED)

add_subdirectory(worker)

target_link_libraries(server PRIVATE 
    ${HTTPSERVER_LIBRARIES}
    ${MICROHTTPD_LIBRARIES}
    CURL::libcurl
    worker
    algorithms
)

target_include_directories(server PRIVATE
    ${HTTPSERVER_INCLUDE_DIRS}
    ${MICROHTTPD_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}
)
