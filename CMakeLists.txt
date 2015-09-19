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

set (Priorytetyzator_SRC
    Priorytetyzator/mainpriorytetyzator.h
    Priorytetyzator/mainpriorytetyzator.cpp
)

set (Klocki_SRC
    Sterowanie_klocki/mainklocki.cpp
    Sterowanie_klocki/mainklocki.h
)

set (Joystick_SRC
    Sterowanie_joystick/mainjoystick.cpp
    Sterowanie_joystick/mainjoystick.h
)

set (Walidator_SRC
    Walidator/mainwalidator.cpp
    Walidator/mainwalidator.h
)

set (Komunikacja_SRC
    Komunikacja/mainkomunikacja.cpp
    Komunikacja/mainkomunikacja.h
)

set (CommonUtils_SRC
    Common-utils/mapelements.cpp
    Common-utils/mapelements.h
    Common-utils/datacollector.h
    Common-utils/threader.h
    Common-utils/threader.cpp
)

add_executable(${PROJECT_NAME}
    main.cpp
    ${GUI_SRC}
    ${GUI_UI}
    ${Rozpoznawator_SRC}
    ${Priorytetyzator_SRC}
    ${Klocki_SRC}
    ${Joystick_SRC}
    ${Komunikacja_SRC}
    ${Walidator_SRC}
    ${CommonUtils_SRC}
)

target_link_libraries(${PROJECT_NAME}
    Qt5::Widgets
    ${OpenCV_LIBS}
    ${aruco_LIBS}
)

