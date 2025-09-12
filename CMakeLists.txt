cmake_minimum_required(VERSION 3.16)

project(SteedDivisionStuff VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
qt_standard_project_setup()

qt_add_executable(SteedDivisionStuff
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        readSupplyThread.cpp
        readSupplyThread.h
        readRepairThread.cpp
        readRepairThread.h
        readPointsThread.cpp
        readPointsThread.h
        writeSupplyThread.cpp
        writeSupplyThread.h
        writeRepairThread.cpp
        writeRepairThread.h
        writePointsThread.cpp
        writePointsThread.h
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
