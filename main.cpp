#include "mainwindow.h"

#include <QApplication>

#include "glog/logging.h"
#include"params/dssystemparam.h"
#include"packagechecker.h"
#include"params/clscommoninput.h"
#include<QSplashScreen>
#include <tchar.h>
#include <windows.h>
#include <DbgHelp.h>
#include"qtCommon/frmmessagebox.h"
#include<vector>

#include "board/Xmodem.h"
#include "board/SerialCom.h"
#include "board/CMDHost.h"
#include<list>
#include<map>
#include<string>

#pragma comment(lib, "user32.lib")


//
//
//

void SetStyle(const QString &styleName);
typedef int(* login)();
bool checkCurrent();

long ApplicationCrashHandler(EXCEPTION_POINTERS *pException);
int main(int argc, char *argv[])
{
    locale::global(locale(""));


//    UINT8 packetBuf[PACKET_BUF_SIZE];
//    memset(packetBuf, 0, PACKET_BUF_SIZE * sizeof(UINT8));
//    OpenPort(1, 115200);
//    XModemCommEngine(SLAVE1_CODE, R_CODE, UPLOAD_DATA_LEN, packetBuf);
//    packetBuf[1] = 255;
//    XModemCommEngine(SLAVE1_CODE, W_CODE, DOWNLOAD_DATA_LEN, packetBuf);
//    ClosePort();


//    dmap.insert(std::pair<int,std::string>(1,"Tom"));
//    dmap.insert(std::pair<int,std::string>(2,"Jack"));
//    dmap.insert(std::pair<int,std::string>(3,"Join"));

//    dmap.insert(1,"1");

//    dmap.insert(1,"Tom");
//   dmap.insert(2,"Jack");
//    dmap.insert(3,"Join");

//    qmap.insert(1,"Tom");
//    qmap.insert(2,"Jack");
//    qmap.insert(3,"Join");

//    std::map<int,std::string>::iterator itormap=dmap.end();

//    QMap<int ,QString>::iterator itor=qmap.end();
//    itor--;
//    QMap<int,QString> tmp;
//    tmp=qmap;
//    qmap.erase(qmap.begin());
//    for(;;itor--)
//    {
//         qDebug()<<itor.key()<<endl;
//         qDebug()<<itor.value()<<endl;

//         if(itor==qmap.begin())
//         break;
//    }
//    qmap.erase(--qmap.end());


//    for(;itormap!=dmap.end();itormap++)
//    {
//       qDebug()<< (*itormap).first<<endl;
//       qDebug()<<((*itormap).second).c_str()<<endl;
//    }
//    std::list<double*>::iterator itor=dlist.begin();
//    for(;itor!=dlist.end();itor++)
//    {
//        qDebug()<<*(*itor)<<endl;
//    }




    SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)ApplicationCrashHandler);
    google::InitGoogleLogging(argv[0]);
    QApplication a(argc, argv);

    if(!checkCurrent())
    {
        frmMessageBox *msg = new frmMessageBox;
        msg->SetMessage("当前软件已开启，请勿重复打开!" , 2);
        msg->exec();
        return 0;
    }

//#ifdef _RELEASE
//    HMODULE hDll=LoadLibraryA("DsDogApi.dll");
//    if(hDll==nullptr)
//    {

//        frmMessageBox *msg = new frmMessageBox;
//        msg->SetMessage("加密狗故障!" , 2);
//        msg->exec();
//        return 0;
//    }
//    login lg=(login)GetProcAddress(hDll,"login");
//    unsigned long  ssss=GetLastError();
//    if(lg==0)
//    {

//        frmMessageBox *msg = new frmMessageBox;
//        msg->SetMessage("加密狗故障2!" , 2);
//        msg->exec();
//        return 0;
//    }
//    int reslogin=lg();
//    if(reslogin!=0)
//    {

//        frmMessageBox *msg = new frmMessageBox;
//        msg->SetMessage("未找到有效的加密狗!" , 2);
//        msg->exec();
//        return 0;
//    }
//#endif

    DSSystemParam::AppPath=QCoreApplication::applicationDirPath();
    FLAGS_max_log_size = 4;
    QDir dir(DSSystemParam::AppPath+"/log");
    if(!dir.exists())FileHelper::makeDir(DSSystemParam::AppPath+"/log");
    google::SetLogDestination(google::GLOG_INFO,  QString(DSSystemParam::AppPath+"/log/dashulogInfo-").toStdString().c_str());
    google::SetLogDestination(google::GLOG_ERROR, QString(DSSystemParam::AppPath+"/log/dashulogErr-").toStdString().c_str());
    LOG(INFO)<<"START"<<endl;
    LOG(ERROR)<<"START"<<endl;
    dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    dir.setSorting(QDir::Size | QDir::Reversed);
    QFileInfoList list = dir.entryInfoList();
    for (int i = 0; i < list.size(); ++i)
    {
        QFileInfo fileInfo = list.at(i);
        QString tmpStr=fileInfo.baseName();
        QStringList tmplst=tmpStr.split("-");
        if(tmplst.size()<2) continue;
        bool b=false;
        tmplst[1].toInt(&b);
        if(!b) continue;
        if(tmplst[1].toInt()<QDateTime::currentDateTime().toString("yyyyMMdd").toInt())
        {
            QFile::remove(fileInfo.absoluteFilePath());
        }
    }


    SetStyle("black");

    QPixmap pix(DSSystemParam::AppPath+"/load.png");
    QSplashScreen splash(pix);
    splash.show();
    a.processEvents();

    PackageChecker * pc=PackageChecker::getInstance();
    try
    {
        pc->init();
    }
    catch(PackageChecker::PackageCheckerException ex)
    {

        splash.finish(nullptr);
        return -1;
    }





    MainWindow w;
    splash.finish(&w);
    w.show();
    return a.exec();
}
void SetStyle(const QString &styleName)
{
    QFile file(QString(":/image/%1.css").arg(styleName));
    file.open(QFile::ReadOnly);
    QString qss = QLatin1String(file.readAll());
    qApp->setStyleSheet(qss);
    qApp->setPalette(QPalette(QColor("#F0F0F0")));
}
bool checkCurrent()
{
    HANDLE m_hMutex  =  CreateMutex(NULL, FALSE,  L"packagechecker" );

    if  (GetLastError()  ==  ERROR_ALREADY_EXISTS)
    {
        CloseHandle(m_hMutex);
        m_hMutex  =  NULL;
        return  false;
    }
    else
        return true;

}


//!analyze -v

long ApplicationCrashHandler(EXCEPTION_POINTERS *pException)
{
    {
        QDir *dmp = new QDir;
        bool exist = dmp->exists("./dmp");
        if(exist == false)
        {
            dmp->mkdir("./dmp");
        }
    }

    QDateTime current_date_time = QDateTime::currentDateTime();
    QString current_date = current_date_time.toString("yyyy_MM_dd_hh_mm_ss");
    QString time =  current_date + ".dmp";
    EXCEPTION_RECORD *record = pException->ExceptionRecord;
    QString errCode(QString::number(record->ExceptionCode, 16));
    QString errAddr(QString::number((uint)record->ExceptionAddress, 16));
    QString errFlag(QString::number(record->ExceptionFlags, 16));
    QString errPara(QString::number(record->NumberParameters, 16));
    qDebug()<< "errCode: "<<errCode;
    qDebug()<< "errAddr: "<<errAddr;
    qDebug()<< "errFlag: "<<errFlag;
    qDebug()<< "errPara: "<<errPara;

    //创建 Dump 文件
    HANDLE hDumpFile = CreateFile((LPCWSTR)QString("./dmp/" + time).utf16(),
                                  GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    if(hDumpFile != INVALID_HANDLE_VALUE)
    {
        //Dump信息
        MINIDUMP_EXCEPTION_INFORMATION dumpInfo;
        dumpInfo.ExceptionPointers = pException;
        dumpInfo.ThreadId = GetCurrentThreadId();
        dumpInfo.ClientPointers = TRUE;
        //写入Dump文件内容
        MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(),hDumpFile, MiniDumpNormal, &dumpInfo, NULL, NULL);
        CloseHandle(hDumpFile);
    }
    else
    {
        qDebug()<< "hDumpFile == null";
    }

    //这里弹出一个错误对话框并退出程序

    frmMessageBox *msg = new frmMessageBox;
    msg->SetMessage(QString("抱歉！程序崩溃,请联系大树公司服务人员..."), 2);
    msg->exec();

    //kill exe

    return EXCEPTION_EXECUTE_HANDLER;
}


