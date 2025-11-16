cmake_minimum_required(VERSION 3.16)

project(TaskProcessingWidgets LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Sql)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Sql)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_library(TaskProcessingWidgets STATIC
  taskprocessingwidgets.cpp
  taskprocessingwidgets.h
  tasksidewidget.h tasksidewidget.cpp
  task_recordings/taskrecordings.h task_recordings/taskrecordings.cpp
  task_recordings/taskrecords.h task_recordings/taskrecords.cpp
  persistance/persistancetaskhelper.h persistance/persistancetaskhelper.cpp
  persistance/databasepersistancetaskhelper.h persistance/databasepersistancetaskhelper.cpp
  task_recordings/taskrecordshelpers.h task_recordings/taskrecordshelpers.cpp
  inits/taskiniter.h inits/taskiniter.cpp
  taskfieldsidewidget.h taskfieldsidewidget.cpp
  ui_event/uieventhandler.h ui_event/uieventhandler.cpp
  inits/databasepreinit.h inits/databasepreinit.cpp
  taskmonthysidewidget.h taskmonthysidewidget.cpp
  extractedprogresstreewidget.h extractedprogresstreewidget.cpp
)

target_link_libraries(TaskProcessingWidgets
  PRIVATE Qt${QT_VERSION_MAJOR}::Widgets CCWidgets Utilities Qt${QT_VERSION_MAJOR}::Sql UiUtils)
target_link_libraries(TaskProcessingWidgets PRIVATE Qt6::Core)
target_link_libraries(TaskProcessingWidgets PRIVATE Qt6::Widgets)
target_link_libraries(TaskProcessingWidgets PRIVATE Qt6::Widgets)
target_link_libraries(TaskProcessingWidgets PRIVATE Qt6::Widgets)

target_include_directories(TaskProcessingWidgets PRIVATE .)
target_compile_definitions(TaskProcessingWidgets PRIVATE TASKPROCESSINGWIDGETS_LIBRARY)
