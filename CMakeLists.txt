cmake_minimum_required(VERSION 3.16)

project(SteedDivisionStuff VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
qt_standard_project_setup()

qt_add_executable(SteedDivisionStuff
        main.cpp
        mainwindow.ui
        mainwindow.cpp
        mainwindow.h
        source/divisionDialog/divisionDialog.ui
        source/divisionDialog/divisionDialog.cpp
        source/divisionDialog/divisionDialog.h
        source/mainThreads/readSupplyThread.cpp
        source/mainThreads/readSupplyThread.h
        source/mainThreads/readRepairThread.cpp
        source/mainThreads/readRepairThread.h
        source/mainThreads/readPointsThread.cpp
        source/mainThreads/readPointsThread.h
        source/mainThreads/writeSupplyThread.cpp
        source/mainThreads/writeSupplyThread.h
        source/mainThreads/writeRepairThread.cpp
        source/mainThreads/writeRepairThread.h
        source/mainThreads/writePointsThread.cpp
        source/mainThreads/writePointsThread.h
        source/divisionDialog/threads/readDivisionsThread.cpp
        source/divisionDialog/threads/readDivisionsThread.h
        source/divisionDialog/threads/ReadDivisionRulesThread.cpp
        source/divisionDialog/threads/ReadDivisionRulesThread.h
        source/divisionDialog/threads/readPacksThread.cpp
        source/divisionDialog/threads/readPacksThread.h
)
target_link_libraries(SteedDivisionStuff PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(SteedDivisionStuff PROPERTIES
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS SteedDivisionStuff
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
