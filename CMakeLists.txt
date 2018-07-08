cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

project(coro VERSION 1.0 LANGUAGES CXX)

message("source dir is: ${PROJECT_SOURCE_DIR}")
message("binary dir is: ${PROJECT_BINARY_DIR}")

message("source dir is: ${coro_SOURCE_DIR}")
message("binary dir is: ${coro_BINARY_DIR}")

message("version is: ${coro_VERSION}")

message(STATUS "We are on a ${CMAKE_SYSTEM_NAME} system")
message(STATUS "The host processor is ${CMAKE_HOST_SYSTEM_PROCESSOR}")
message(STATUS "The compiler is ${CMAKE_CXX_COMPILER_ID}")

find_package(Boost 1.67 REQUIRED COMPONENTS coroutine)
message(STATUS "Boost include dir: ${Boost_INCLUDE_DIR}")
message(STATUS "Boost libs: ${Boost_LIBRARIES}")

find_package(OpenSSL REQUIRED)
message(STATUS "OpenSSL include dir: ${OPENSSL_INCLUDE_DIR}")
message(STATUS "OpenSSL libs: ${OPENSSL_LIBRARIES}")

add_subdirectory(drivers)

set(SOURCES
    main.cpp
)

add_executable (coro ${SOURCES})
target_include_directories(coro PUBLIC ${Boost_INCLUDE_DIR} ${OPENSSL_INCLUDE_DIR})
target_link_libraries(coro PUBLIC ${Boost_LIBRARIES} ${OPENSSL_LIBRARIES} drivers)
