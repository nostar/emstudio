######################################################################
# Automatically generated by qmake (2.01a) Sat Feb 16 17:37:58 2013
######################################################################

TEMPLATE = lib

DEPENDPATH += .
INCLUDEPATH += . ../../core/src
CONFIG += plugin
QT -= gui

win32-x-g++ {
	TARGET = ../../../core/plugins/freeemsplugin
	win32:QMAKE_LFLAGS += -shared
	message("Building for win32-x-g++")
	INCLUDEPATH += /home/michael/QtWin/libs/qwt/include /home/michael/QtWin/libs/qjson/include
	LIBS += -L/home/michael/QtWin/libs/qwt/lib -lqwt -L/home/michael/QtWin/libs/qjson/lib -lqjson0
	LIBS += -L/home/michael/QtWin/lib
	DEFINES += GIT_COMMIT=$$system(git describe --dirty=-DEV --always)
	DEFINES += GIT_HASH=$$system(git log -n 1 --pretty=format:%H)
}
unix {
	TARGET = ../../core/plugins/freeemsplugin
	target.path = /usr/share/emstudio/plugins
	INSTALLS += target
}
# Input
HEADERS += ../../core/src/datapacketdecoder.h \
	   fedatapacketdecoder.h \
           fetable2ddata.h \
	   ../../core/src/table2ddata.h \
	   ../../core/src/table3ddata.h \
	   ../../core/src/tabledata.h \
	   ../../core/src/emscomms.h \
	   ../../core/src/datafield.h \
	   ../../core/src/memorymetadata.h \
           fetable3ddata.h \
           freeemscomms.h \
           serialport.h \
           serialrxthread.h \
    fememorymetadata.h
SOURCES += fedatapacketdecoder.cpp \
           fetable2ddata.cpp \
           fetable3ddata.cpp \
           freeemscomms.cpp \
           serialport.cpp \
	   ../../core/src/datafield.cpp \
	   ../../core/src/tabledata.cpp \
           serialrxthread.cpp \
    fememorymetadata.cpp
