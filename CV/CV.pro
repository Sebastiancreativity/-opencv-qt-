QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    photo.cpp \
    reco.cpp \
    src/bif.cpp \
    src/eigen_faces.cpp \
    src/face_alignment.cpp \
    src/face_basic.cpp \
    src/facemark.cpp \
    src/facemarkAAM.cpp \
    src/facemarkLBF.cpp \
    src/facerec.cpp \
    src/fisher_faces.cpp \
    src/getlandmarks.cpp \
    src/lbph_faces.cpp \
    src/mace.cpp \
    src/predict_collector.cpp \
    src/regtree.cpp \
    src/trainFacemark.cpp \
    train.cpp \
    widget.cpp

HEADERS += \
    face.hpp \
    face/bif.hpp \
    face/face_alignment.hpp \
    face/facemark.hpp \
    face/facemarkAAM.hpp \
    face/facemarkLBF.hpp \
    face/facemark_train.hpp \
    face/facerec.hpp \
    face/mace.hpp \
    face/predict_collector.hpp \
    photo.h \
    reco.h \
    src/face_alignmentimpl.hpp \
    src/face_utils.hpp \
    src/precomp.hpp \
    train.h \
    widget.h

FORMS += \
    widget.ui

TRANSLATIONS += \
    CV_zh_CN.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH += opencv/opencv-4.10.0/include/opencv2 \
            opencv/opencv_build/install/include/opencv2\
            opencv/opencv_build/install/include \
            opencv/opencv_build

LIBS += opencv/opencv_build/lib/lib*.a

RESOURCES += \
    cv.qrc
