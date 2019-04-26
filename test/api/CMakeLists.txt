add_library(api_test STATIC
    database.cpp
)
target_link_libraries(api_test
    json
)
