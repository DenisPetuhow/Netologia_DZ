cmake_minimum_required(VERSION 3.22.0)
project(bd_cmake LANGUAGES CXX)

# Устанавливаем стандарт C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Подключаем исходники libpqxx
# — используем прямые слеши и кавычки, т.к. путь может содержать пробелы
add_subdirectory("D:/Programm/2. Education/2. Netologia/1. C++/Netologia/1_DZ/Lesson_6/Lesson_6_5/DZ" libpqxx-build)

# Добавляем собственный исполняемый файл
add_executable(bd_cmake bd.cpp)

# Линкуем с библиотекой pqxx (цель определяется CMakeLists.txt внутри libpqxx)
target_link_libraries(bd_cmake PRIVATE pqxx)