cmake_minimum_required(VERSION 3.16)

project(MainPage LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Network)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_library(MainPage STATIC
  mainpagewidget.cpp
  mainpagewidget.h
  NasaAPOD/nasaapodfetcher.h NasaAPOD/nasaapodfetcher.cpp
  NasaAPOD/nasaapod_error.hpp
  NasaAPOD/NasaAPODData.h
  floatinglabelhelper.h floatinglabelhelper.cpp
  NasaAPOD/nasaapodattrwidget.h NasaAPOD/nasaapodattrwidget.cpp
  defaultimagehandler.h defaultimagehandler.cpp
  init/apoddatadefault.h init/apoddatadefault.cpp
)

target_link_libraries(MainPage PRIVATE
  Qt${QT_VERSION_MAJOR}::Network
  Qt${QT_VERSION_MAJOR}::Widgets
  CCWidgets)
target_link_libraries(MainPage PUBLIC Utilities UiUtils)
target_link_libraries(MainPage PRIVATE Qt6::Widgets)

target_include_directories(MainPage PUBLIC .)
target_compile_definitions(MainPage PRIVATE MAINPAGE_LIBRARY)
