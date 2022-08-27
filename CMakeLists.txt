cmake_minimum_required(VERSION 3.14)

project(ImageProcessing VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

make_directory("images")

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Quick REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Quick REQUIRED)

set(PROJECT_SOURCES
        main.cpp
        imageprovider.h imageprovider.cpp
        imageprocessing.h imageprocessing.cpp
        Tools/tools.h Tools/tools.cpp
        Tools/zoom.h Tools/zoom.cpp
        Tools/hand.h Tools/hand.cpp
        Tools/pen.h Tools/pen.cpp
        Tools/smartbrush.h Tools/smartbrush.cpp
        Tools/chanvese.h Tools/chanvese.cpp
        Filters/filters.h Filters/filters.cpp
        Filters/borderhighlight.h Filters/borderhighlight.cpp
        Filters/medianfilter.h Filters/medianfilter.cpp
        Filters/gaussfilter.h Filters/gaussfilter.cpp
        Filters/meanfilter.h Filters/meanfilter.cpp
        Utils/matrix.h
        Utils/maskpoint.h
        qml.qrc
        images/image1.png
        images/image2.png
        images/image3.png
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(ImageProcessing
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET ImageProcessing APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(ImageProcessing SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(ImageProcessing
          ${PROJECT_SOURCES}
        )
    endif()
endif()

target_compile_definitions(ImageProcessing
  PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(ImageProcessing
  PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Quick)

set_target_properties(ImageProcessing PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_import_qml_plugins(ImageProcessing)
    qt_finalize_executable(ImageProcessing)
endif()
