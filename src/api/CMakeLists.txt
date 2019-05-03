include_directories(${PROJECT_SOURCE_DIR}/include)

add_compile_options(
    -fvisibility=hidden
    -fvisibility-inlines-hidden
)

add_library(adblock SHARED
    c_api.cpp
    database.cpp
)
target_link_libraries(adblock
    core
)
target_link_options(adblock
    PRIVATE
        -Wl,--no-undefined
        -Wl,--exclude-libs=ALL
)
set_target_properties(adblock PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)

install(TARGETS adblock
    LIBRARY DESTINATION lib
)

install(
    FILES ${PROJECT_SOURCE_DIR}/include/adblock/database.h
          ${PROJECT_SOURCE_DIR}/include/adblock/database.hpp
          ${PROJECT_SOURCE_DIR}/include/adblock/dll.h
    DESTINATION include/adblock
)
