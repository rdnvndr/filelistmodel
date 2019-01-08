cmake_minimum_required(VERSION 3.10)
project(filelistmodel VERSION 1.0.0)

# Файлы для сборки
file(GLOB HEADERS   "*.h")
file(GLOB SOURCES   "*.cpp")
file(GLOB FORMS     "*.ui")
file(GLOB RESOURCES "*.qrc")

# Поддержка Qt Framework
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC_OPTIONS -no-compress)
set(ignoreMe "${QT_QMAKE_EXECUTABLE}")

# Сборка проекта
add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES} ${FORMS} ${RESOURCES})

find_package(Qt5Core)
target_link_libraries(${PROJECT_NAME} Qt5::Core)

find_package(Qt5Widgets)
target_link_libraries(${PROJECT_NAME} Qt5::Widgets)

find_package(Qt5Concurrent)
target_link_libraries(${PROJECT_NAME} Qt5::Concurrent)
