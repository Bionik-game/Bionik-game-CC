cmake_minimum_required(VERSION 2.8.11)
project(Bionik-game)
add_definitions(-std=c++11)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
find_package(Qt5Widgets)
find_package(OpenCV REQUIRED)

set(CMAKE_MODULE_PATH "/usr/local/lib/cmake;${CMAKE_MODULE_PATH}")

find_package(aruco REQUIRED)

set (GUI_SRC
    GUI/mainwindow.cpp
    GUI/mainwindow.h
)
QT5_WRAP_UI(GUI_UI
    GUI/mainwindow.ui
)

set (Rozpoznawator_SRC
    Rozpoznawator/mainrozpoznawator.cpp
    Rozpoznawator/mainrozpoznawator.h
    Rozpoznawator/block.h
    Rozpoznawator/block.cpp
)

set (Klocki_SRC
    Sterowanie_klocki/mainklocki.cpp
    Sterowanie_klocki/mainklocki.h
)

set (CommonUtils_SRC
    Common-utils/mapelements.cpp
    Common-utils/mapelements.h
    Common-utils/datacollector.h
)

add_executable(${PROJECT_NAME}
    main.cpp
    ${GUI_SRC}
    ${GUI_UI}
    ${Rozpoznawator_SRC}
    ${Klocki_SRC}
    ${CommonUtils_SRC}
)


target_link_libraries(${PROJECT_NAME}
    Qt5::Widgets
    ${OpenCV_LIBS}
    ${aruco_LIBS}
)

