cmake_minimum_required(VERSION 3.16)

project(Utilities LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS
            Core Network Concurrent Gui)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS
            Core Network Concurrent Gui)

add_library(Utilities SHARED
  Utilities_global.h
  cached_proxy.hpp  NetworkQueryHelper.hpp
  cached_policy_error.hpp
  netimagefetch.h netimagefetch.cpp
  url_reader.h url_reader.cpp
)

target_link_libraries(Utilities PRIVATE
  Qt${QT_VERSION_MAJOR}::Core
  Qt${QT_VERSION_MAJOR}::Gui
  Qt${QT_VERSION_MAJOR}::Network
  Qt${QT_VERSION_MAJOR}::Concurrent)

target_include_directories(Utilities PUBLIC .)

target_compile_definitions(Utilities PRIVATE UTILITIES_LIBRARY)
