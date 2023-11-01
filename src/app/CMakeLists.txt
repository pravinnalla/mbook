cmake_minimum_required(VERSION 3.13)

project(MBook VERSION 1.0.0 LANGUAGES CXX)


add_executable(MBook
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    dbmanager.cpp
    dbmanager.h
    threadedcsvimport.cpp
    threadedcsvimport.h
    pdfprint.cpp
    pdfprint.h    
    AppResources.qrc
)

target_link_libraries(MBook PRIVATE Qt5::Widgets Qt5::Sql hpdf)

INSTALL(TARGETS MBook DESTINATION bin)
INSTALL(TARGETS MBook
        LIBRARY
        DESTINATION lib)
