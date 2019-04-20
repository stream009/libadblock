add_library(parser
    rule_builder.cpp
    parser.cpp
)

target_include_directories(parser PRIVATE
    /home/stream/work/adblock_parser/include
)

target_link_libraries(parser
    core
    rule
    adblock_parser
)
