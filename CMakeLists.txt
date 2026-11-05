cmake_minimum_required(VERSION 3.10)

project(AIP VERSION "1.0")

set(CMAKE_CXX_STANDARD 17)          # Стандарт языка C++17
set(CMAKE_CXX_STANDARD_REQUIRED ON) # Стандарт обязателен

include(CTest) # Подключение модуля тестирования CTest

if(NOT DEFINED ENV{WORKSDK})
    message(FATAL_ERROR "Environment variable WORKSDK is not set.")
endif()

# Подпроекты
add_subdirectory(BasicUT)
add_subdirectory(EnvironmentTest)