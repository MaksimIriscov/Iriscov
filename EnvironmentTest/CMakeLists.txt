cmake_minimum_required(VERSION 3.10)
project(EnvironmentTest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    add_compile_options(/Zc:__cplusplus)
endif()

set(EnvironmentTest_SRC
    src/main.cpp
)

add_executable(EnvironmentTest
    ${EnvironmentTest_SRC}
)