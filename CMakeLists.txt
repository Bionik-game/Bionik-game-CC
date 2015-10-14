cmake_minimum_required(VERSION 2.8.11)
project(Bionik-game)
add_definitions(-std=c++11)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_MODULE_PATH "/usr/local/lib/cmake;${CMAKE_MODULE_PATH}")
find_package(Qt5Widgets REQUIRED)
find_package(OpenCV 3.0.0 REQUIRED)
find_package(aruco REQUIRED)

#-ljsoncpp -lmicrohttpd -ljsonrpccpp-common -ljsonrpccpp-server

find_library(JsonRpcCppCommon_LIB "jsonrpccpp-common")
find_library(JsonRpcCppClient_LIB "jsonrpccpp-client")
find_library(JsonCpp_LIB "jsoncpp")
find_library(microhttpd_LIB "microhttpd")

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
    Sterowanie_joystick/qjoystick.cpp
    Sterowanie_joystick/qjoystick.h
)

set (Walidator_SRC
    Walidator/mainwalidator.cpp
    Walidator/mainwalidator.h
)

set (Komunikacja_SRC
    Komunikacja/stubclient.h
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

include_directories(
    ${OpenCV_INCLUDE_DIR}
    ${aruco_INCLUDE_DIR}
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

message(XXX ${JsonRpcCpp})

target_link_libraries(${PROJECT_NAME}
    Qt5::Widgets
    ${OpenCV_LIBS}
    ${aruco_LIBS}
    ${JsonRpcCppCommon_LIB}
    ${JsonRpcCppClient_LIB}
    ${JsonCpp_LIB}
    ${microhttpd_LIB}
    SDL
)

