cmake_minimum_required(VERSION 4.1)
project(15)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(Fifteen src/main.cpp
        src/model.h
        src/view.h
        src/controller.h
)
target_link_libraries(Fifteen
        Qt::Core
        Qt::Gui
        Qt::Widgets
)

