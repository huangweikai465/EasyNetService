QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

macx {
    TARGET = ENSClient
    DEFINES += MACOS
    LIBS += -lpthread
    ICON = logo.icns
}
unix : !macx {
    DEFINES += LINUX
    LIBS += -lpthread
}
win32 {
    DEFINES += WIN32
    LIBS += -lIphlpapi -lws2_32
    RC_ICONS = logo.ico
}

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

TRANSLATIONS += zh_EN.ts

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ../Src/Client/ClientSocketAction.cpp \
    ../Src/Client/EasyNetServiceClient.cpp \
    ../Src/Log/Log.cpp \
    ../Src/Server/EasyNetServiceServer.cpp \
    ../Src/Server/FileOpt.cpp \
    ../Src/Server/ServerSocketAction.cpp \
    ../Src/Server/ServiceNode.cpp \
    ../Src/Socket/IpInfo.cpp \
    ../Src/Socket/SocketService.cpp \
    ../Src/Socket/TCPClient.cpp \
    ../Src/Socket/TCPServer.cpp \
    ../Src/Socket/UDPClient.cpp \
    ../Src/Socket/UDPServer.cpp \
    ../Src/cJSON/JsonOpt.cpp \
    ../Src/cJSON/cJSON.cpp \
    editterminalcmd.cpp \
    main.cpp \
    mainwindow.cpp \
    configtcpport.cpp \
    configapp.cpp \
    selectcmd.cpp \
    addappconfig.cpp \
    version.cpp

HEADERS += \
    ../Src/Client/ClientSocketAction.h \
    ../Src/Client/EasyNetServiceClient.h \
    ../Src/Log/Log.h \
    ../Src/Server/EasyNetServiceServer.h \
    ../Src/Server/FileOpt.h \
    ../Src/Server/ServerSocketAction.h \
    ../Src/Server/ServiceNode.h \
    ../Src/Socket/IpInfo.h \
    ../Src/Socket/SocketInclude.h \
    ../Src/Socket/SocketService.h \
    ../Src/Socket/TCPClient.h \
    ../Src/Socket/TCPServer.h \
    ../Src/Socket/UDPClient.h \
    ../Src/Socket/UDPServer.h \
    ../Src/cJSON/JsonOpt.h \
    ../Src/cJSON/cJSON.h \
    editterminalcmd.h \
    mainwindow.h \
    configtcpport.h \
    configapp.h \
    selectcmd.h \
    addappconfig.h \
    version.h

FORMS += \
    editterminalcmd.ui \
    mainwindow.ui \
    configtcpport.ui \
    configapp.ui \
    selectcmd.ui \
    addappconfig.ui \
    version.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

#DISTFILES += \
#    ../Src/CMakeLists.txt

DISTFILES += \
    zh_CN.qm

RESOURCES += \
    EnsRC.qrc
