set(TARGET uri_test)

add_executable(${TARGET}
    main.cpp
    parser.cpp
)
target_link_libraries(${TARGET} boost_unit_test_framework uri)
