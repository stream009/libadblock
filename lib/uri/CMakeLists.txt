set(TARGET uri)

find_package(Boost 1.66 REQUIRED)

add_library(${TARGET} STATIC
    parser.cpp
    uri.cpp
)
target_link_libraries(${TARGET} ${Boost_LIBRARIES})
target_include_directories(${TARGET}
    PRIVATE "$ENV{HOME}/src/spirit/include" # boost 1.66 has bug
)
set_property(TARGET ${TARGET} PROPERTY
    POSITION_INDEPENDENT_CODE ON
)

add_subdirectory(test)
