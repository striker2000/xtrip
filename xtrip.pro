QT += network

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    mapwidget.cpp \
    latlon.cpp \
    tilesmap.cpp \
    tilesloaderthread.cpp \
    overlays.cpp \
    point.cpp \
    pointmarker.cpp \
    cursor.cpp \
    pointdialog.cpp

HEADERS += \
    mainwindow.h \
    mapwidget.h \
    latlon.h \
    tilesmap.h \
    tilessource.h \
    osmtilessource.h \
    tilesloaderthread.h \
    overlays.h \
    point.h \
    pointmarker.h \
    cursor.h \
    pointdialog.h

FORMS += \
    pointdialog.ui
