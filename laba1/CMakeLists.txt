cmake_minimum_required(VERSION 3.16)

project(FINANCE VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Charts)  # Добавлено Charts
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Core Charts)  # Добавлено Charts
find_package(SQLite3 REQUIRED)

set(PROJECT_SOURCES
    main.cpp
    MainWindow.cpp
    MainWindow.h
    MainWindow.ui
    FinanceService.h
    DatabaseService.h
    FinanceService.cpp
    DatabaseService.cpp
    FinanceEntry.h
    ExpenseRecord.h
    IncomeRecord.h
    Budget.cpp
    Budget.h
    ActionHandler.h
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(FINANCE
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(FINANCE SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(FINANCE
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(FINANCE PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(FINANCE PRIVATE Qt${QT_VERSION_MAJOR}::Core)  # Связывание с Core
target_link_libraries(FINANCE PRIVATE Qt${QT_VERSION_MAJOR}::Charts)  # Связывание с Charts
target_link_libraries(FINANCE PRIVATE SQLite::SQLite3)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
if(${QT_VERSION} VERSION_LESS 6.1.0)
    set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.FINANCE)
endif()
set_target_properties(FINANCE PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS FINANCE
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(FINANCE)
endif()
