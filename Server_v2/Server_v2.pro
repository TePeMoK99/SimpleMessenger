QT -= gui
QT += core sql network

CONFIG += c++17 console
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp \
        src/CRUD/connectionmanager.cpp \
        src/CRUD/executor.cpp \
        src/CRUD/manipulator.cpp \
        src/CRUD/processor.cpp \
        src/CRUD/selector.cpp \
        src/tcpclient.cpp \
        src/tcpserver.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    src/CRUD/connectionmanager.h \
    src/CRUD/crudmapper.h \
    src/CRUD/crudtypes.h \
    src/CRUD/executor.h \
    src/CRUD/manipulator.h \
    src/CRUD/processor.h \
    src/CRUD/selector.h \
    src/group.h \
    src/tcpclient.h \
    src/tcpserver.h \
    src/types.h
