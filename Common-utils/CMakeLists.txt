project(Bionik-game)
cmake_minimum_required(VERSION 2.8.11)
add_definitions(-std=c++11)


set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
find_package(Qt5Widgets)
find_package(Qt5Network)

find_package(SDL REQUIRED)

include_directories(

        ${SDL_INCLUDE_DIR}
)

set (Joystick_SRC
    Joystick/joypick.cpp
    Joystick/joypick.h
    Joystick/qjoystick.h
    Joystick/qjoystick.cpp
    Joystick/mapelements.h
    Joystick/mapelements.cpp
)


add_executable(${PROJECT_NAME}
    Joystick/main.cpp
    ${Joystick_SRC}
)
 message(STATUS "SDL: ${SDL_LIBRARY}")
target_link_libraries(${PROJECT_NAME}
    Qt5::Widgets
    Qt5::Network
    ${SDL_LIBRARY}
)

