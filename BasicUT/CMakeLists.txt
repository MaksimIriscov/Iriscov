project(BasicUT)

set(BasicUT_SRCS
    src/main.cpp
    src/Converter.cpp
    src/Converter.h
)

add_executable(BasicUT
	${BasicUT_SRCS}
)

# Tests
set(BasicUT_TESTS_SRCS
    tests/testMain.cpp
    src/Converter.cpp
)


add_executable(BasicUTRunner 
    ${BasicUT_TESTS_SRCS}
)