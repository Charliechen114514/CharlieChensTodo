cmake_minimum_required(VERSION 3.16)

project(UiUtils LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Core Gui)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Core Gui)

add_library(UiUtils SHARED
  UiUtils_Global.h
  parser_adapter/markdownparser.h parser_adapter/markdownparser.cpp
  parser_adapter/markdown_parser_privates.h parser_adapter/markdown_parser_privates.cpp

  dialogs/textselectordialog.h dialogs/textselectordialog.cpp
  dialogs/commons.h dialogs/commons.cpp
  dialogs/datedialog.h dialogs/datedialog.cpp
  dialogs/yearmonthdialog.h dialogs/yearmonthdialog.cpp
)

target_link_libraries(UiUtils PRIVATE
  Qt${QT_VERSION_MAJOR}::Core
  Qt${QT_VERSION_MAJOR}::Gui
  Qt${QT_VERSION_MAJOR}::Widgets
  CCWidgets PlanParser)

target_include_directories(UiUtils PUBLIC .)

target_compile_definitions(UiUtils PRIVATE UiUtils_LIBRARY)
