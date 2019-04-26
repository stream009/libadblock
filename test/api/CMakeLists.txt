add_library(api_test STATIC
    database.cpp
    c_api.cpp
)
target_link_libraries(api_test
    json
)
