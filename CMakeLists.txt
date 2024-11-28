cmake_minimum_required(VERSION 3.5)

project(Memorona VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets LinguistTools Test REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets LinguistTools Test REQUIRED)

set(TS_FILES Memorona_ca_ES.ts)

set(PROJECT_SOURCES
        ClickableCard.h
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        ${TS_FILES}
        images.qrc
)

add_library(Memorona OBJECT ranking.h ranking.cpp
    difficultylevel.h difficultylevel.cpp)
target_link_libraries(Memorona PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

add_executable(OnaMemorona
    ${PROJECT_SOURCES}
    rankingdialog.h rankingdialog.cpp rankingdialog.ui
    i18n.qrc
)

qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})

target_link_libraries(OnaMemorona PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Memorona)

set_target_properties(OnaMemorona PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)


enable_testing(true)

add_executable(TestRanking test/test_ranking.cpp)

add_test(NAME TestRanking COMMAND TestRanking)

target_link_libraries(TestRanking PRIVATE Qt::Test Memorona)
