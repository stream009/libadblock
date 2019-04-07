set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} \
    -std=c99 \
	-pedantic -Wall -Wextra -Werror -Wwrite-strings \
	-Wstrict-prototypes -Wmissing-prototypes -Wnested-externs \
	-Wredundant-decls -Wcast-qual -Wcast-align -Winline \
	-Wno-unused-parameter -Wno-overlength-strings"
)
add_library(reg_dom
    regdom.c
)
