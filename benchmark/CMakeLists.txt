set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wconversion -Wsign-conversion")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -Wfatal-errors")

add_definitions(-DPROJECT_ROOT=\"${CMAKE_SOURCE_DIR}\")
include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(memory_usage
    memory_usage.cpp
)
target_link_libraries(memory_usage
    adblock
    stdc++fs
)

add_custom_target(run_memory_benchmark
    COMMAND valgrind --tool=massif --massif-out-file=massif.out ./memory_usage && ms_print massif.out
)
