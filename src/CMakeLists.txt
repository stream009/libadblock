set(Boost_USE_STATIC_LIBS ON)
find_package(Boost 1.57 REQUIRED
             COMPONENTS regex timer)
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARIES_DIRS})

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC -fvisibility=hidden")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wconversion -Wsign-conversion")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -Wfatal-errors")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -fvisibility=hidden")

set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--no-undefined")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--exclude-libs=ALL")

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib)

add_subdirectory(core)
add_subdirectory(rule)
add_subdirectory(index)
add_subdirectory(pattern)
add_subdirectory(parser)
add_subdirectory(lib/reg_dom)
add_subdirectory(lib/uri)

add_library(adblock SHARED
    api.cpp
)
target_link_libraries(adblock
    core
)
set_target_properties(adblock PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
)
install(TARGETS adblock
    LIBRARY DESTINATION lib
)
install(FILES api.h
    DESTINATION include/adblock
)
