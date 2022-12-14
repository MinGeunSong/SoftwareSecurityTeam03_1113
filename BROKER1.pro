QT       += core gui sql
QT       += widgets
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
QMAKE_CXXFLAGS += -std=c++0x
QMAKE_CXXFLAGS += -z execstack -fno-stack-protector -z norelro -g -O0

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    deposit.cpp \
    main.cpp \
    mainafterlogin.cpp \
    mainwindow.cpp \
    signupwindow.cpp \
    transaction.cpp \
    request.cpp \
    buyer_transaction.cpp \
    seller_transaction.cpp \
    secondhand_id.cpp

HEADERS += \
    deposit.h \
    mainafterlogin.h \
    mainwindow.h \
    signupwindow.h \
    transaction.h \
    request.h \
    buyer_transaction.h \
    seller_transaction.h \
    secondhand_id.h

FORMS += \
    deposit.ui \
    mainafterlogin.ui \
    mainwindow.ui \
    signupwindow.ui \
    transaction.ui \
    request.ui \
    buyer_transaction.ui \
    seller_transaction.ui \
    secondhand_id.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
