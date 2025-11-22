cmake_minimum_required(VERSION 3.16)

project(CharliesTodo VERSION 0.1 LANGUAGES CXX)
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>"
                    "$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

# Dependencies
message("Finding the Project Neccessary Dependencccies...")
set(SETTINGS_FILE "${CMAKE_SOURCE_DIR}/cmake/settings.cmake")
if(EXISTS ${SETTINGS_FILE})
    message(STATUS "Including optional settings: ${SETTINGS_FILE}")
    include(${SETTINGS_FILE})
else()
    message(STATUS "Optional settings.cmake not found, skipping.")
endif()
include(cmake/third_party.cmake)

add_subdirectory(UiUtils)
add_subdirectory(Utilities)
add_subdirectory(MainPage)
add_subdirectory(TaskProcessing)

set(PROJECT_SOURCES
        main.cpp
        todomainwindow.cpp
        todomainwindow.h
        todomainwindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Core)
    find_package(Qt6 REQUIRED COMPONENTS Core)
    find_package(Qt6 REQUIRED COMPONENTS Widgets)
    find_package(Qt6 REQUIRED COMPONENTS Core)
    find_package(Qt6 REQUIRED COMPONENTS Core)
    find_package(Qt6 REQUIRED COMPONENTS Widgets)
    qt_add_executable(CharliesTodo
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        sources/source.qrc
        init_session.cpp
        interfaces/IPagedWidget.h
        init/todopreinithelper.h init/todopreinithelper.cpp
        init/todoinititem.h init/todoinititem.cpp
        loggy/loggerbackend.h loggy/loggerbackend.cpp
        loggy/uiwindow/loggersubwindow.h loggy/uiwindow/loggersubwindow.cpp
        loggy/uiwindow/loggerwindowbackend.h loggy/uiwindow/loggerwindowbackend.cpp
        markdown_preview/markdownpreviewwindow.h markdown_preview/markdownpreviewwindow.cpp

    )
else()
    add_executable(CharliesTodo
        ${PROJECT_SOURCES})
endif()

if(WIN32)
    target_sources(CharliesTodo PRIVATE resources.rc)
endif()

target_link_libraries(CharliesTodo
    PRIVATE Qt${QT_VERSION_MAJOR}::Widgets CCWidgets
    PUBLIC MainPage TaskProcessingWidgets CCLogger)
target_link_libraries(CharliesTodo PRIVATE Qt6::Core)
target_link_libraries(CharliesTodo PRIVATE Qt6::Widgets)
target_link_libraries(CharliesTodo PRIVATE Qt6::Core)
target_link_libraries(CharliesTodo PRIVATE Qt6::Core)
target_link_libraries(CharliesTodo PRIVATE Qt6::Widgets)

if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.CharliesTodo)
endif()
set_target_properties(CharliesTodo PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS CharliesTodo
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(CharliesTodo)
endif()



