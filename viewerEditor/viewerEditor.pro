QT += widgets

HEADERS =    \
recorder.h \
    viewerEditor.h \
    viewerConsole/datefilter.h \
    TextEdit.h \
    TimeEdit.h \
    Combo.h
SOURCES =   main.cpp \ 
    recorder.cpp \
    viewerEditor.cpp \
    viewerConsole/datefilter.cpp \
    TextEdit.cpp \
    TimeEdit.cpp \
    Combo.cpp

# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/widgets/calendarwidget
INSTALLS += target

RESOURCES = icons.qrc
