cmake_minimum_required(VERSION 3.22.0)
project(bd_cmake)

# Укажите путь к PostgreSQL
set(PostgreSQL_ROOT "C:/Program Files/PostgreSQL/17")

# Найдите PostgreSQL автоматически
find_package(PostgreSQL REQUIRED)

add_executable(bd_cmake main.cpp)
add_subdirectory(libpqxx libpqxx-build)
target_compile_features(bd_cmake PRIVATE cxx_std_17)
target_link_libraries(bd_cmake pqxx)