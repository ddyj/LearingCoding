cmake_minimum_required(VERSION 3.5)

project(QT_E_albums LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check http://doc.qt.io/qt-5/deployment-android.html for more information.
# They need to be set before the find_package(Qt5 ...) call.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()



find_package(Qt5 COMPONENTS Widgets Multimedia REQUIRED)

set(QRC_SOURCE_FILES Resources/rc.qrc)
qt5_add_resources(QRC_FILES ${QRC_SOURCE_FILES})

if(ANDROID)
  add_library(QT_E_albums SHARED
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    wizard.cpp
    wizard.h
    wizard.ui

    confirmpage.cpp
    confirmpage.h
    confirmpage.ui
    prosetpage.cpp
    prosetpage.h
    prosetpage.ui
    protree.h
    protree.cpp
    protree.ui
    protreewidget.h
    protreewidget.cpp
    protreeitem.h
    protreeitem.cpp
    const.h

    protreethread.cpp
    protreethread.h
    removeprodialog.h
    removeprodialog.cpp
    removeprodialog.ui
    opentreethead.cpp
    opentreethead.h
    picshow.cpp
    picshow.h
    picshow.ui
    picbutton.cpp
    picbutton.h

    slideshowdlg.cpp
    slideshowdlg.h
    slideshowdlg.ui

    picstatebtn.h
    picstatebtn.cpp

    picanimationwid.h
    picanimationwid.cpp
    prelistwid.h
    prelistwid.cpp
    prelistitem.h
    prelistitem.cpp
         ${QRC_FILES}
  )
else()
  add_executable(QT_E_albums
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    wizard.cpp
    wizard.h
    wizard.ui

    confirmpage.cpp
    confirmpage.h
    confirmpage.ui
    prosetpage.cpp
    prosetpage.h
    prosetpage.ui
    protree.h
    protree.cpp
    protree.ui
    protreewidget.h
    protreewidget.cpp
    protreeitem.h
    protreeitem.cpp
    const.h

    protreethread.cpp
    protreethread.h
    removeprodialog.h
    removeprodialog.cpp
    removeprodialog.ui
    opentreethead.cpp
    opentreethead.h
    picshow.cpp
    picshow.h
    picshow.ui
    picbutton.cpp
    picbutton.h
    slideshowdlg.cpp
    slideshowdlg.h
    slideshowdlg.ui
    picstatebtn.h
    picstatebtn.cpp

    picanimationwid.h
    picanimationwid.cpp
    prelistwid.h
    prelistwid.cpp
    prelistitem.h
    prelistitem.cpp
         ${QRC_FILES}
  )
endif()


target_link_libraries(QT_E_albums PRIVATE Qt5::Widgets Qt5::Multimedia)
