add_library(option_test STATIC
    script.cpp
    image.cpp
    stylesheet.cpp
    object.cpp
    xmlhttprequest.cpp
    object_sub_request.cpp
    sub_document.cpp
    media.cpp
    font.cpp
    other.cpp
    third_party.cpp
    domain.cpp
    site_key.cpp
    popup.cpp
    generic_hide.cpp
    generic_block.cpp
    websocket.cpp
    document.cpp
    elem_hide.cpp
    ping.cpp
    webrtc.cpp
    csp.cpp
)
target_link_libraries(option_test
    main
)


