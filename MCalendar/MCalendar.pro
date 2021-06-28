
QT += widgets

HEADERS =   window.h \
    MCalendar.h \
    MDate.h \
    fears.h \
    MGroupCalendar.h \
    CoordinateTransformation.h \
    MoonPhases.h \
    newRdvWin.h \
    ../viewerEditor/recorder.h

SOURCES =   main.cpp \ 
	    window.cpp \
    MCalendar.cpp \
    MDate.cpp \
    MGroupCalendar.cpp \
    CoordinateTransformation.cpp \
    MoonPhases.cpp \
    newRdvWin.cpp \
    ../viewerEditor/recorder.cpp

TRANSLATIONS = MCalendar_fr.ts

RESOURCES     = icons.qrc
