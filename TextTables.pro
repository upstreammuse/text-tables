TEMPLATE = app
!win32:QMAKE_CXXFLAGS += -Werror -Wextra
HEADERS += \
   MainWindow.h \
   TextTable.h
SOURCES += \
   main.cpp \
   MainWindow.cpp \
   TextTable.cpp
FORMS += \
   MainWindow.ui
TARGET = TextTables
