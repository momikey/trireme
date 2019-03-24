cmake_minimum_required(VERSION 3.0.0)
project(trireme VERSION 0.1.0)

include(CTest)
enable_testing()

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Boost REQUIRED COMPONENTS unit_test_framework)

set(TRIREME_INCLUDES include/convert.hpp include/ternary_math.hpp include/hexad.hpp include/word.hpp)
set(TRIREME_IMPL_INCLUDES include/detail/convert_impl.hpp)
set(TRIREME_SOURCES src/hexad.cpp src/word.cpp)

set(PROJECT_SOURCES ${TRIREME_SOURCES} ${TRIREME_INCLUDES} ${TRIREME_IMPL_INCLUDES})

add_library(libtrireme STATIC ${TRIREME_SOURCES})
target_include_directories(libtrireme PRIVATE include)

add_executable(trireme src/main.cpp)
target_include_directories(trireme PRIVATE include)
target_link_libraries(trireme libtrireme)

add_executable(trireme_test tests/testmain.cpp)
target_compile_definitions(trireme_test PRIVATE BOOST_TEST_DYN_LINK)
target_link_libraries(trireme_test libtrireme Boost::unit_test_framework)
add_test(trireme_test trireme_test)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
