find_package(Boost 1.57 REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARIES_DIRS})
set(Boost_USE_STATIC_LIBS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Wall -Wextra -Wconversion -Wsign-conversion")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -Wfatal-errors")

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib)

add_subdirectory(core)
add_subdirectory(rule)
add_subdirectory(index)
add_subdirectory(pattern)
add_subdirectory(parser)
add_subdirectory(lib/reg_dom)
add_subdirectory(lib/uri)
add_subdirectory(api)
