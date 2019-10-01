set(DIR_SOURCES
    shell.cpp
)

set(DIR_INCLUDES
    shell.hpp
)

add_library(trireme_shell STATIC ${DIR_SOURCES})
target_include_directories(trireme_shell PRIVATE ../../include)
target_link_libraries(trireme_shell PUBLIC trireme_assembler fmt::fmt replxx)