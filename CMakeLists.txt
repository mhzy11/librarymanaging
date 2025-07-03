cmake_minimum_required(VERSION 3.16)

project(library1 VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 仅保留Qt Widgets和Core组件
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Core Sql Charts)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Core Sql Charts)

set(PROJECT_SOURCES
    main.cpp
    book.cpp book.h
    borrowrecord.cpp borrowrecord.h
    library.cpp library.h
    querythread.cpp querythread.h
    reader.cpp reader.h
    form.h form.cpp form.ui
    registering.h registering.cpp registering.ui
    login.h login.cpp login.ui
    globals.h
    control1.h control1.cpp control1.ui
    control2.h control2.cpp control2.ui
    search.h search.cpp search.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(library1 MANUAL_FINALIZATION ${PROJECT_SOURCES}


        BookInfo.h
        recouse.qrc
        recommended.h recommended.cpp recommended.ui

    )
else()
    if(ANDROID)
        add_library(library1 SHARED ${PROJECT_SOURCES})
    else()
        add_executable(library1 ${PROJECT_SOURCES})
    endif()
endif()

# 仅链接Qt Widgets和Core库
target_link_libraries(library1 PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Sql
    Qt${QT_VERSION_MAJOR}::Charts
)

# 以下保留原始配置
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.library1)
endif()
set_target_properties(library1 PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS library1
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(library1)
endif()


