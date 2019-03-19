cmake_minimum_required(VERSION 3.0.0)
project(trireme VERSION 0.1.0)

include(CTest)
enable_testing()

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(TRIREME_INCLUDES include/convert.hpp include/ternary_math.hpp include/hexad.hpp include/word.hpp)
set(TRIREME_IMPL_INCLUDES detail/convert_impl.hpp)
set(TRIREME_SOURCES src/main.cpp src/hexad.cpp src/word.cpp)

set(PROJECT_SOURCES ${TRIREME_SOURCES} ${TRIREME_INCLUDES} ${TRIREME_IMPL_INCLUDES})

add_executable(trireme ${TRIREME_SOURCES})
target_include_directories(trireme PRIVATE include)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
