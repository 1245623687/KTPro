QT       += core gui
QT       += core xml
QT       += sql
QT += concurrent
QT += serialport
QT  += network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TRANSLATIONS = English.ts
CONFIG += c++11

unix: LIBS += -lbiodaq

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += GLOG_NO_ABBREVIATED_SEVERITIES

DEFINES += FLAG_KONGTOU

DEFINES+=FALG_PHE


RC_ICONS = dashu.ico

DEFINES += IMG_TYPE_RGB
#DEFINES += IMG_TYPE_GRAY

QMAKE_CXXFLAGS_RELEASE = $$QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO
QMAKE_LFLAGS_RELEASE = $$QMAKE_LFLAGS_RELEASE_WITH_DEBUGINFO

win32:LIBS += -lDbgHelp

VERSION = 2.2.0112

QMAKE_TARGET_PRODUCT = 烟条外观
QMAKE_TARGET_COMPANY = 大树智能
QMAKE_TARGET_DESCRIPTION = 主程序
QMAKE_TARGET_COPYRIGHT = 大树智能烟条外观




win32:CONFIG(release, debug|release): DEFINES += _RELEASE
else:win32:CONFIG(debug, debug|release): DEFINES += _DEBUG
# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    COM/basecom.cpp \
    COM/dlgphotoelectricitycfg.cpp \
#    Camera/cCarShuixingMer231.cpp \
#    Camera/cHKUSBcamera.cpp \
    Camera/camerafactory.cpp \
    Camera/ccamerabase.cpp \
    Camera/ccarshuixingmer231c.cpp \
    Camera/dlgcameraconfig.cpp \
    Camera/csimucanera.cpp \
    CheckOperator/DSIppImage.cpp \
    CheckOperator/Onnx.cpp \
    CheckOperator/imgpro.cpp \
    CheckOperator/imgprocontrol.cpp \
    CheckOperator/imgtobacco.cpp \
    CheckOperator/imgtobaccocontrol.cpp \
    IO/AAEONGPIO.cpp \
    IO/advantenchgpio.cpp \
    IO/ced_bc09_2a_fun.cpp \
    IO/ced_bc09_2a_gpio.cpp \
    IO/eqgpio.cpp\
    IO/fitgpio.cpp \
    IO/ltgfunc.cpp\
    IO/ciobase.cpp \
    IO/iodaqnavi.cpp \
    ImageHandle/opencvimage.cpp \
    board/CRC.cpp \
    board/SerialCom.cpp \
    board/Update_SerialPort.cpp \
    board/Xmodem.cpp \
    board/testcase.cpp \
    common/IniFile.cpp \
    common/SystemSetting.cpp \
    common/XmlHelper.cpp \
    common/filehelper.cpp \
    common/infile2.cpp \
    common/systemrunlog.cpp \
    config/dlgbrandadd.cpp \
    config/dlgbrandmanage.cpp \
    config/dlgsystemconfig.cpp \
    config/dlgsysteminit.cpp \
    controls/graphicscheckoperatoritem.cpp \
    controls/graphicsitembase.cpp \
    controls/graphicsitemconfig.cpp \
    controls/graphicsitemscene.cpp \
    controls/graphicslineitem.cpp \
    controls/graphicspixmapitem.cpp \
    controls/graphicsrectitem.cpp \
    controls/graphicsscene.cpp \
    controls/graphicsscenecapture.cpp \
    controls/graphicsscenecheckoperatormain.cpp \
    controls/graphicssceneconfig.cpp \
    controls/graphicsscenemain.cpp \
    controls/graphicsscenerec.cpp \
    controls/graphicssinglelineitem.cpp \
    controls/graphicsview.cpp \
    controls/linehandlerect.cpp \
    controls/sizehandlerect.cpp \
    database/ClsRecordFile.cpp \
    database/dlgoperatorlog.cpp \
    database/dsfilerecord.cpp \
    database/mainrecord.cpp \
    database/recorddialog.cpp \
    dlgCheckOperator/dlgassociatedinspect.cpp \
    dlgCheckOperator/dlgassociatedinspect2.cpp \
    dlgCheckOperator/dlgbrokenlineinspect.cpp \
    dlgCheckOperator/dlgcheckoperator.cpp \
    dlgCheckOperator/dlgcircleinspect.cpp \
    dlgCheckOperator/dlgdefectinspect.cpp \
    dlgCheckOperator/dlgdefectinspect2.cpp \
    dlgCheckOperator/dlgdoublestayinspect.cpp \
    dlgCheckOperator/dlgedgepositioning.cpp \
    dlgCheckOperator/dlggraymeasurement.cpp \
    dlgCheckOperator/dlgmatchtemplate.cpp \
    dlgCheckOperator/dlgstayinspect.cpp \
    dlgCheckOperator/dlgDLobjectdetect.cpp \
    dlgbase.cpp \
    dlgimagerecord.cpp \
    kcontrols/dlgkeyboard.cpp \
    kcontrols/kcontrolsbase.cpp \
    kcontrols/klabel.cpp \
    kcontrols/klineedit.cpp \
    kcontrols/klineeditkb.cpp \
    kcontrols/kpercentage.cpp \
    kcontrols/kprogressbar.cpp \
    kcontrols/krunbutton.cpp \
    kcontrols/kslidebutton.cpp \
    kcontrols/kspeedpanel.cpp \
    kcontrols/kspinboxkb.cpp \
    kcontrols/ktableview.cpp \
    main.cpp \
    mainwindow.cpp \
    packagechecker.cpp \
    params/clscameraconfig.cpp \
    params/clscamerefile.cpp \
    params/clscommoninput.cpp \
    params/dslastconfig.cpp \
    params/dsoptions.cpp \
    params/dssystemparam.cpp \
    params/clsphototelecconfig.cpp \
    qtCommon/frmmessagebox.cpp \
    qtCommon/iconhelper.cpp \
    thread/kthreadbase.cpp \
    thread/kthreadcameraconfig.cpp \
    thread/kthreaddatabase.cpp \
    thread/kthreadgrab.cpp \
    thread/kthreadproc.cpp \
    thread/kthreadsaveimage.cpp \
    TCP/cbaseserver.cpp \
    user/dlgusermanage.cpp \
    user/userinfo.cpp \
    user/userinfosqlitehelper.cpp

HEADERS += \
    COM/basecom.h \
    COM/dlgphotoelectricitycfg.h \
#    Camera/cCarShuixingMer231.h \
#    Camera/cHKUSBcamera.h \
    Camera/camerafactory.h \
    Camera/ccamerabase.h \
    Camera/ccarshuixingmer231c.h \
    Camera/dlgcameraconfig.h \
    Camera/csimucanera.h \
    CheckOperator/CheckOperator.hpp \
    CheckOperator/CheckOperatorControl.hpp \
    CheckOperator/DSIppImage.h \
    CheckOperator/Onnx.h \
    CheckOperator/ImageBase.hpp \
    CheckOperator/imgpro.h \
    CheckOperator/imgprocontrol.h \
    CheckOperator/imgtobacco.h \
    CheckOperator/imgtobaccocontrol.h \
    IO/AAEONGPIO.h \
    IO/advantenchgpio.h \
    IO/ced_bc09_2a_fun.h \
    IO/ced_bc09_2a_gpio.h \
    IO/eqgpio.h\
    IO/fitgpio.h \
    IO/ltgfunc.h\
    IO/ciobase.h \
    IO/iodaqnavi.h \
    ImageHandle/opencvimage.h \
    board/CMDHost.h \
    board/CRC.h \
    board/SerialCom.h \
    board/Update_SerialPort.h \
    board/Xmodem.h \
    board/xddefs.h \
    common/IniFile.h \
    common/SystemSetting.h \
    common/XmlHelper.h \
    common/filehelper.h \
    common/infile2.h \
    common/systemrunlog.h \
    config/dlgbrandadd.h \
    config/dlgbrandmanage.h \
    config/dlgsystemconfig.h \
    config/dlgsysteminit.h \
    controls/dsgraphicsitemimage.hpp \
    controls/graphicscheckoperatoritem.h \
    controls/graphicsitembase.h \
    controls/graphicsitemconfig.h \
    controls/graphicsitemscene.h \
    controls/graphicslineitem.h \
    controls/graphicspixmapitem.h \
    controls/graphicsrectitem.h \
    controls/graphicsscene.h \
    controls/graphicsscenecapture.h \
    controls/graphicsscenecheckoperatormain.h \
    controls/graphicssceneconfig.h \
    controls/graphicsscenemain.h \
    controls/graphicsscenerec.h \
    controls/graphicssinglelineitem.h \
    controls/graphicsview.h \
    controls/linehandlerect.h \
    controls/sizehandlerect.h \
    database/ClsRecordFile.h \
    database/dlgoperatorlog.h \
    database/dsfilerecord.h \
    database/mainrecord.h \
    database/recorddialog.h \
    dlgCheckOperator/dlgassociatedinspect.h \
    dlgCheckOperator/dlgassociatedinspect2.h \
    dlgCheckOperator/dlgbrokenlineinspect.h \
    dlgCheckOperator/dlgcheckoperator.h \
    dlgCheckOperator/dlgcircleinspect.h \
    dlgCheckOperator/dlgdefectinspect.h \
    dlgCheckOperator/dlgdefectinspect2.h \
    dlgCheckOperator/dlgdoublestayinspect.h \
    dlgCheckOperator/dlgedgepositioning.h \
    dlgCheckOperator/dlggraymeasurement.h \
    dlgCheckOperator/dlgmatchtemplate.h \
    dlgCheckOperator/dlgstayinspect.h \
    dlgCheckOperator/dlgDLobjectdetect.h \
    dlgbase.h \
    dlgimagerecord.h \
    dsbase.h \
    kcontrols/dlgkeyboard.h \
    kcontrols/kcontrolsbase.h \
    kcontrols/klabel.h \
    kcontrols/klineedit.h \
    kcontrols/klineeditkb.h \
    kcontrols/kpercentage.h \
    kcontrols/kprogressbar.h \
    kcontrols/krunbutton.h \
    kcontrols/kslidebutton.h \
    kcontrols/kspeedpanel.h \
    kcontrols/kspinboxkb.h \
    kcontrols/ktableview.h \
    mainwindow.h \
    packagechecker.h \
    params/clscameraconfig.h \
    params/clscamerefile.h \
    params/clscommoninput.h \
    params/dslastconfig.h \
    params/dsoptions.h \
    params/dssystemparam.h \
    params/clsphototelecconfig.h \
    qtCommon/frmmessagebox.h \
    qtCommon/iconhelper.h \
    qtCommon/myhelper.h \
    thread/kthreadbase.h \
    thread/kthreadcameraconfig.h \
    thread/kthreaddatabase.h \
    thread/kthreadgrab.h \
    thread/kthreadproc.h \
    thread/kthreadsaveimage.h \
    TCP/cbaseserver.h \
    user/dlgusermanage.h \
    user/userinfo.h \
    user/userinfosqlitehelper.h

FORMS += \
    COM/dlgphotoelectricitycfg.ui \
    Camera/dlgcameraconfig.ui \
    config/dlgbrandadd.ui \
    config/dlgbrandmanage.ui \
    config/dlgsystemconfig.ui \
    config/dlgsysteminit.ui \
    database/dlgoperatorlog.ui \
    database/recorddialog.ui \
    dlgCheckOperator/dlgassociatedinspect.ui \
    dlgCheckOperator/dlgassociatedinspect2.ui \
    dlgCheckOperator/dlgbrokenlineinspect.ui \
    dlgCheckOperator/dlgcheckoperator.ui \
    dlgCheckOperator/dlgcircleinspect.ui \
    dlgCheckOperator/dlgdefectinspect.ui \
    dlgCheckOperator/dlgdefectinspect2.ui \
    dlgCheckOperator/dlgdoublestayinspect.ui \
    dlgCheckOperator/dlgedgepositioning.ui \
    dlgCheckOperator/dlggraymeasurement.ui \
    dlgCheckOperator/dlgmatchtemplate.ui \
    dlgCheckOperator/dlgstayinspect.ui \
    dlgCheckOperator/dlgDLobjectdetect.ui \
    dlgimagerecord.ui \
    kcontrols/dlgkeyboard.ui \
    mainwindow.ui \
    qtCommon/frmmessagebox.ui \
    user/dlgusermanage.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target



#tinyxml2
LIBS += -L$$PWD/tinyxml2/ -ltinyxml2
INCLUDEPATH+= $$PWD/tinyxml2


win32:CONFIG(release, debug|release): DEFINES += _RELEASE
else:win32:CONFIG(debug, debug|release): DEFINES += _DEBUG

#opencv
#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/opencv/opencv341/lib/ -lopencv_world341
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/opencv/opencv341/lib/ -lopencv_world341d
#CONFIG(release, debug|release): LIBS += -L$$PWD/opencv/opencv341/lib/ -lopencv_world341
#else:CONFIG(debug, debug|release): LIBS += -L$$PWD/opencv/opencv341/lib/ -lopencv_world341d

#INCLUDEPATH += $$PWD/opencv/opencv341/include \
#            += $$PWD/opencv/opencv341/include/opencv \
#            += $$PWD/opencv/opencv341/include/opencv2
#DEPENDPATH += $$PWD/opencv/opencv341/include





CONFIG(release, debug|release): LIBS += -L$$PWD/opencv/opencv451/lib/ -lopencv_core451
else:CONFIG(debug, debug|release): LIBS += -L$$PWD/opencv/opencv451/lib/ -lopencv_core451d
CONFIG(release, debug|release): LIBS += -L$$PWD/opencv/opencv451/lib/ -lopencv_imgcodecs451
else:CONFIG(debug, debug|release): LIBS += -L$$PWD/opencv/opencv451/lib/ -lopencv_imgcodecs451d
CONFIG(release, debug|release): LIBS += -L$$PWD/opencv/opencv451/lib/ -lopencv_dnn451
else:CONFIG(debug, debug|release): LIBS += -L$$PWD/opencv/opencv451/lib/ -lopencv_dnn451d
CONFIG(release, debug|release): LIBS += -L$$PWD/opencv/opencv451/lib/ -lopencv_imgproc451
else:CONFIG(debug, debug|release): LIBS += -L$$PWD/opencv/opencv451/lib/ -lopencv_imgproc451d
CONFIG(release, debug|release): LIBS += -L$$PWD/opencv/opencv451/lib/ -lopencv_video451
else:CONFIG(debug, debug|release): LIBS += -L$$PWD/opencv/opencv451/lib/ -lopencv_video451d
CONFIG(release, debug|release): LIBS += -L$$PWD/opencv/opencv451/lib/ -lopencv_videoio451
else:CONFIG(debug, debug|release): LIBS += -L$$PWD/opencv/opencv451/lib/ -lopencv_videoio451d
INCLUDEPATH +=$$PWD/opencv/opencv451/include\
            += $$PWD/opencv/opencv451/include/opencv2
DEPENDPATH += $$PWD/opencv/opencv451/include


#IPP
LIBS += -L$$PWD/IPP/lib/ -lippcc
LIBS += -L$$PWD/IPP/lib/ -lippccmt
LIBS += -L$$PWD/IPP/lib/ -lippch
LIBS += -L$$PWD/IPP/lib/ -lippchmt
LIBS += -L$$PWD/IPP/lib/ -lippcore
LIBS += -L$$PWD/IPP/lib/ -lippcoremt
LIBS += -L$$PWD/IPP/lib/ -lippcv
LIBS += -L$$PWD/IPP/lib/ -lippcvmt
LIBS += -L$$PWD/IPP/lib/ -lippdc
LIBS += -L$$PWD/IPP/lib/ -lippdc
LIBS += -L$$PWD/IPP/lib/ -lippi
LIBS += -L$$PWD/IPP/lib/ -lippimt
LIBS += -L$$PWD/IPP/lib/ -lipps
LIBS += -L$$PWD/IPP/lib/ -lippsmt
LIBS += -L$$PWD/IPP/lib/ -lippvm
LIBS += -L$$PWD/IPP/lib/ -lippvmmt

INCLUDEPATH += $$PWD/IPP/include
DEPENDPATH += $$PWD/IPP/include


##onnx11.1
LIBS += -L$$PWD/onnxruntime-win-x64-gpu-1.11.1/lib/ -lonnxruntime
LIBS += -L$$PWD/onnxruntime-win-x64-gpu-1.11.1/lib/ -lonnxruntime_providers_cuda
LIBS += -L$$PWD/onnxruntime-win-x64-gpu-1.11.1/lib/ -lonnxruntime_providers_shared
LIBS += -L$$PWD/onnxruntime-win-x64-gpu-1.11.1/lib/ -lonnxruntime_providers_tensorrt
INCLUDEPATH+= $$PWD/onnxruntime-win-x64-gpu-1.11.1/include
#cuda11.1
INCLUDEPATH+="C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v11.1/include"



#onnx1.6.0
#LIBS += -L$$PWD/onnxruntime-win-x64-gpu-1.6.0/lib/ -lonnxruntime
#INCLUDEPATH+= $$PWD/onnxruntime-win-x64-gpu-1.6.0/include
##cuda10.2
#INCLUDEPATH+="C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v10.2/include"




#vld
win32:{
CONFIG(debug, debug|release){
LIBS += -L$$PWD/vld/lib/Win64/ -lvld
INCLUDEPATH += $$PWD/vld/include
DEPENDPATH += $$PWD/vld/include
    }
}


#研华GPIO驱动
DISTFILES +=
win32: LIBS += -L$$PWD/SUSI4/lib/x64/ -lSusi4

INCLUDEPATH += $$PWD/SUSI4/include
DEPENDPATH += $$PWD/SUSI4/include

#研华高速
DISTFILES +=
INCLUDEPATH += $$PWD/Advantech/include
DEPENDPATH += $$PWD/Advantech/include


#研扬驱动
DISTFILES +=
win32: LIBS += -L$$PWD/IO/ -laaeonEAPI

INCLUDEPATH += $$PWD/IO
DEPENDPATH += $$PWD/IO






##AVT相机
#win32: LIBS += -L$$PWD/cameras/Vimba/VimbaCPP/Lib/Win64/ -lVimbaCPP
#INCLUDEPATH += $$PWD/cameras/Vimba
#DEPENDPATH += $$PWD/cameras/Vimba


#DHcamera
#LIBS += -L$$PWD/cameras/shuixing/lib/ -lGxIAPICPPEx
#INCLUDEPATH+= $$PWD/cameras/shuixing/include

#DHcameraVC
LIBS += -L$$PWD/cameras/shuixingVC/lib/ -lGxIAPI
INCLUDEPATH+= $$PWD/cameras/shuixingVC/include

#HKcamera
#LIBS += -L$$PWD/cameras/HK/Lib/Win64/ -lGxIAPICPPEx
#INCLUDEPATH+= $$PWD/cameras/HK/Include

#glog
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/glog/lib/ -lglog
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/glog/lib/ -lglogd

INCLUDEPATH += $$PWD/glog/include
DEPENDPATH += $$PWD/glog/include

RESOURCES += \
    Icons/Icons.qrc \
    keyboardicons/Icon.qrc \
    rc.qrc
