QT       += core gui
QT += widgets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    BorrowControl.cpp \
    CancelHoldControl.cpp \
    CatalogueControl.cpp \
    FictionBook.cpp \
    Item.cpp \
    Librarian.cpp \
    LibrarySystem.cpp \
    Loan.cpp \
    LoginControl.cpp \
    Magazine.cpp \
    Movie.cpp \
    NonFictionBook.cpp \
    Patron.cpp \
    PlaceHoldControl.cpp \
    ReturnControl.cpp \
    SystemAdministrator.cpp \
    User.cpp \
    VideoGame.cpp \
    ViewAccountStatusControl.cpp \
    catalogcard.cpp \
    itemdetaildialog.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    BorrowControl.h \
    CancelHoldControl.h \
    CatalogueControl.h \
    FictionBook.h \
    Item.h \
    Librarian.h \
    LibrarySystem.h \
    Loan.h \
    LoginControl.h \
    Magazine.h \
    Movie.h \
    NonFictionBook.h \
    Patron.h \
    PlaceHoldControl.h \
    ReturnControl.h \
    SystemAdministrator.h \
    User.h \
    VideoGame.h \
    ViewAccountStatusControl.h \
    catalogcard.h \
    itemdetaildialog.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
