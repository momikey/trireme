cmake_minimum_required(VERSION 3.0.0)
project(trireme VERSION 0.1.0)

include(CTest)
enable_testing()

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(PROJECT_SOURCES src/main.cpp src/convert.cpp include/convert.hpp include/ternary_math.hpp)
set(PROJECT_IMPL_INCLUDES detail/convert_impl.hpp)

add_executable(trireme src/main.cpp)
target_include_directories(trireme PRIVATE include)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
