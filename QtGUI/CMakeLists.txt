set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)


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

find_package(Qt5 COMPONENTS Widgets LinguistTools REQUIRED)

set(TS_FILES QtGUI_zh_CN.ts)

add_executable(ENSClientGUI
  main.cpp
  mainwindow.cpp
  mainwindow.h
  mainwindow.ui
  configtcpport.cpp
  configtcpport.h
  configtcpport.ui
  configapp.cpp
  configapp.h
  configapp.ui
  addappconfig.cpp
  addappconfig.h
  addappconfig.ui
  selectcmd.cpp
  selectcmd.h
  selectcmd.ui
  ${TS_FILES}
)
  
target_link_libraries(ENSClientGUI PRIVATE Qt5::Widgets ${ENSSERVERLIBNAME} ${ENSCLIENTLIBNAME})

qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
