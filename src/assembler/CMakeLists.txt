set(DIR_SOURCES
    assembler.cpp
)

set(DIR_INCLUDES
    assembler.hpp
    detail/actions.hpp
    detail/directives.hpp
    detail/firstpass_actions.hpp
    detail/grammar.hpp
    detail/instruction_actions.hpp
    detail/instructions.hpp
    detail/state.hpp
    detail/tokens.hpp
)

add_library(trireme_assembler STATIC ${DIR_SOURCES})
target_include_directories(trireme_assembler PRIVATE ../../include/)
target_link_libraries(trireme_assembler PUBLIC fmt::fmt)