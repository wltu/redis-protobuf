cmake_minimum_required(VERSION 3.5)

set(REDIS_PROTOBUF_VERSION "0.1.0")
message(STATUS "redis-protobuf version: ${REDIS_PROTOBUF_VERSION}")

project(redis-protobuf LANGUAGES CXX VERSION ${REDIS_PROTOBUF_VERSION})

set(REDIS_PROTOBUF_DEFAULT_BUILD_TYPE "Release")
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE ${REDIS_PROTOBUF_DEFAULT_BUILD_TYPE} CACHE STRING "Set build type" FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "RelWithDebInfo" "MinSizeRel")
endif()
message(STATUS "redis-protobuf build type: ${CMAKE_BUILD_TYPE}")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++23 -Wall -Wextra -Werror -fPIC -Wno-unused-parameter")

set(PROJECT_SOURCE_DIR ${PROJECT_SOURCE_DIR}/src/sw/redis-protobuf)

file(GLOB PROJECT_SOURCE_FILES "${PROJECT_SOURCE_DIR}/*.cpp" "${PROJECT_SOURCE_DIR}/*.cc")

set(SHARED_LIB shared)

add_library(${SHARED_LIB} SHARED ${PROJECT_SOURCE_FILES})

# protobuf dependency
find_path(PROTOBUF_HEADER google)
target_include_directories(${SHARED_LIB} PUBLIC ${PROTOBUF_HEADER})
find_package(Protobuf REQUIRED CONFIG COMPONENTS protoc)

target_link_libraries(${SHARED_LIB} 
    PRIVATE 
    protobuf::libprotobuf  # Replaces ${Protobuf_LIBRARIES}
    protobuf::libprotoc    # Replaces ${Protobuf_PROTOC_LIBRARIES}
)

set_target_properties(${SHARED_LIB} PROPERTIES OUTPUT_NAME ${PROJECT_NAME} VERSION 1.0.0 SOVERSION 1)

set_target_properties(${SHARED_LIB} PROPERTIES CLEAN_DIRECT_OUTPUT 1)

option(BUILD_TESTS "Build tests" ON)

if(BUILD_TESTS)
    enable_testing()

    # Find redis++
    # Assuming it installs a cmake config or we can use find_package
    find_package(redis++ REQUIRED)
    find_package(hiredis REQUIRED)

    # Test source files
    file(GLOB TEST_SOURCE_FILES "test/src/sw/redis-protobuf/*.cpp")

    # Find protoc
    find_program(PROTOC_EXECUTABLE protoc REQUIRED)

    set(PROTO_SRC "${CMAKE_CURRENT_BINARY_DIR}/example.pb.cc")
    set(PROTO_HDR "${CMAKE_CURRENT_BINARY_DIR}/example.pb.h")

    add_custom_command(
        OUTPUT "${PROTO_SRC}" "${PROTO_HDR}"
        COMMAND ${PROTOC_EXECUTABLE}
        ARGS --cpp_out "${CMAKE_CURRENT_BINARY_DIR}" -I "${CMAKE_SOURCE_DIR}/docker" "${CMAKE_SOURCE_DIR}/docker/example.proto"
        DEPENDS "${CMAKE_SOURCE_DIR}/docker/example.proto"
        COMMENT "Generating C++ files for example.proto"
    )

    # Add test executable
    add_executable(redis_protobuf_test ${TEST_SOURCE_FILES} "${PROTO_SRC}")

    # Include directories
    target_include_directories(redis_protobuf_test PRIVATE src ${CMAKE_CURRENT_BINARY_DIR})

    # Link libraries
    target_link_libraries(redis_protobuf_test PRIVATE 
        redis++
        protobuf::libprotobuf
        hiredis
    )
endif()

include(GNUInstallDirs)

# Install shared lib.
install(TARGETS ${SHARED_LIB}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
