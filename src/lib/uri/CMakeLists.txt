set(TARGET uri)

find_package(Boost 1.67 REQUIRED)

add_library(${TARGET} STATIC
    parser.cpp
    uri.cpp
)
target_link_libraries(${TARGET} ${Boost_LIBRARIES})
set_property(TARGET ${TARGET} PROPERTY
    POSITION_INDEPENDENT_CODE ON
)

add_subdirectory(test)
