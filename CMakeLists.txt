cmake_minimum_required(VERSION 3.0.0)
project(trireme VERSION 0.1.0)

include(CTest)
enable_testing()

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Boost REQUIRED COMPONENTS unit_test_framework)
find_package(pegtl)

set(TRIREME_INCLUDES
    include/convert.hpp
    include/ternary_math.hpp
    include/hexad.hpp
    include/word.hpp
    include/registers.hpp
    include/memory.hpp
    include/opcode.hpp
    include/flags.hpp
    include/cpu.hpp
    include/io.hpp
    include/debug_io.hpp
)

set(TRIREME_IMPL_INCLUDES include/detail/convert_impl.hpp)

set(TRIREME_SOURCES
    src/hexad.cpp
    src/word.cpp
    src/registers.cpp
    src/memory.cpp
    src/opcode.cpp
    src/flags.cpp
    src/cpu.cpp
    src/io.cpp
    src/debug_io.cpp
)

set(PROJECT_SOURCES ${TRIREME_SOURCES} ${TRIREME_INCLUDES} ${TRIREME_IMPL_INCLUDES})

add_compile_options(-Wall)

add_library(libtrireme STATIC ${TRIREME_SOURCES})
target_include_directories(libtrireme PRIVATE include thirdparty/include)

add_subdirectory(src/assembler)

add_executable(trireme src/main.cpp)
target_include_directories(trireme PRIVATE include)
target_link_libraries(trireme libtrireme trireme_assembler taocpp::pegtl)

set(TRIREME_TESTS tests/ternary_math_test.cpp tests/convert_test.cpp tests/hexad_test.cpp)
add_executable(trireme_test tests/testmain.cpp ${TRIREME_TESTS})
target_include_directories(trireme_test PRIVATE include thirdparty/include)
target_compile_definitions(trireme_test PRIVATE BOOST_TEST_DYN_LINK)
target_link_libraries(trireme_test libtrireme Boost::unit_test_framework taocpp::pegtl)
add_test(trireme_test trireme_test)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
