set(DIR_SOURCES
    assembler.cpp
)

set(DIR_INCLUDES
    assembler.hpp
    detail/grammar.hpp
)

add_library(trireme_assembler STATIC ${DIR_SOURCES})
target_include_directories(trireme_assembler PRIVATE ../../include/ ../../thirdparty/include)