cmake_minimum_required(VERSION 2.6)

project(Ruin)

# Version Number
#set(Ruin_VERSION_MAJOR 0)
#set(Ruin_VERSION_MINOR 1)
#
#configure_file("${PROJECT_SOURCE_DIR}/gen/ruin_config.g" "${PROJECT_BINARY_DIR}/ruin_config.h")
#
#add_subdirectory(lib/gl3w)
#add_subdirectory(lib/glfw)
#add_subdirectory(lib/glm)
#add_subdirectory(lib/rapidjson)
#
#include_directories("${PROJECT_BINARY_DIR}")
#include_directories("${PROJECT_SOURCE_DIR}/lib/gl3w/include")
#include_directories("${PROJECT_SOURCE_DIR}/lib/glfw/include")
#include_directories("${PROJECT_SOURCE_DIR}/lib/glm")
#include_directories("${PROJECT_SOURCE_DIR}/lib/rapidjson/include")
#
#file(GLOB SOURCES "*.cpp")
#
#add_executable(Ruin ${SOURCES})
#
#target_link_libraries(Ruin gl3w)
#target_link_libraries(Ruin glfw)

add_subdirectory(code)