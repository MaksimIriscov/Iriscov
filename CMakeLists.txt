cmake_minimum_required(VERSION 3.10) # Минимальная версия CMake генератора

project(EnvironmentTest) # Наименование проекта

# Список файлов с исходниками
set(EnvironmentTest_SRC
    src/main.cpp
)

# Исходники будут скомпилированы в исполняемый файл
add_executable(EnvironmentTest
	${EnvironmentTest_SRC}
)