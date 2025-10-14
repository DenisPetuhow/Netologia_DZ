cmake_minimum_required(VERSION 3.22.0)

# ВАЖНО: До project()!
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "D:/dev/vcpkg/scripts/buildsystems/vcpkg.cmake" 
        CACHE FILEPATH "Toolchain file")
endif()

project(test_wt)
set(CMAKE_CXX_STANDARD 20)

# Найдите Wt
find_package(Wt CONFIG REQUIRED)

# Укажите путь к PostgreSQL
set(PostgreSQL_ROOT "C:/Program Files/PostgreSQL/17")

# Найдите PostgreSQL автоматически
find_package(PostgreSQL REQUIRED)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} 
    PRIVATE 
    Wt::Wt 
    Wt::Dbo 
    Wt::HTTP 
    Wt::DboPostgres
)
