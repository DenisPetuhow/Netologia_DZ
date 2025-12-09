cmake_minimum_required(VERSION 3.16)

project(DZ LANGUAGES CXX) # Имя проекта и язык (C++)

# Включаем автоматическую кодогенерацию Qt (MOC, UIC, RCC)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Задаем стандарт C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- БЛОК ПОДКЛЮЧЕНИЯ МОДУЛЕЙ ---

# 1. Сначала ищем любой установленный Qt (либо 6, либо 5).
# Эта команда определит переменную QT_VERSION_MAJOR (равную 5 или 6).
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
# 2. Теперь, зная версию, ищем конкретные компоненты (Core, Sql, Network).
# ${QT_VERSION_MAJOR} автоматически подставит нужную цифру.
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Sql Network)

# Создаем исполняемый файл из main.cpp
add_executable(DZ
  main.cpp
)

# 3. Линкуем (привязываем) библиотеки к нашему приложению.
# Используем синтаксис Qt${QT_VERSION_MAJOR}::ModuleName
target_link_libraries(DZ PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Sql
    Qt${QT_VERSION_MAJOR}::Network
  )

# --- БЛОК УСТАНОВКИ (из вашего примера) ---
include(GNUInstallDirs)
install(TARGETS DZ
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
