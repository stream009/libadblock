set(TARGET uri_test)

find_package(Boost 1.66 REQUIRED COMPONENTS unit_test_framework)

#add_definitions(-DBOOST_TEST_DYN_LINK)

add_executable(${TARGET}
    main.cpp
    parser.cpp
)
target_link_libraries(${TARGET} ${Boost_LIBRARIES} uri)
