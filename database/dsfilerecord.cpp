

#include "dsfilerecord.h"

#include"packagechecker.h"
#include"common/filehelper.h"
#include<QString>

//#pragma comment(lib, "./recordinfo/RecordInfo.lib")


QString ToStr33(int number, int size)
{
    return QString("%1").arg(number, size, 10, QChar('0'));
}

DsFileRecord::DsFileRecord()
{

}

void DsFileRecord::getCurRecord(MainRecord& record)
{
    record.m_No=getCurMainRecordNo();
    record.m_Date=QDate::currentDate().toString("yyyy-MM-dd");
    record.m_Brand=DSSystemParam::BrandName;

    //record.m_NumNg[0]=PackageChecker::getInstance()->RunParam_CalcNumNg;
    memcpy(record.m_NumNg,PackageChecker::getInstance()->RunParam_CalcNumNgCams,sizeof (int)*CAMERANUM_MAX);
    record.m_Shift=static_cast<int>(DSSystemParam::getCurrentShift());
    record.m_NumAll=PackageChecker::getInstance()->RunParam_CalcNumAll;
    record.m_StrShift=record.m_Shift==0?"早班":(record.m_Shift==1?"中班":"晚班");

}

void DsFileRecord::loadMainRecord(QString dirPath,std::vector<MainRecord>& lst)
{
    QVector<QString> files;
    FileHelper::getAllFilesInFolder(dirPath,files);

    for(int idx=0;idx<files.size();++idx)
    {
        QFileInfo fi(files[idx]);
        QString baseName= fi.baseName();
        if(baseName.length()!=9||baseName==getCurMainRecordNo())
            continue;

        QString fn=dirPath+"/"+files[idx];
        char name[128]={0};
        int allNum=0;
        int ngNum[CAMERANUM_MAX]={0};

        if(0!=getSimpleRecord(fn.toStdString(),name,&allNum,ngNum))
        {
            continue;
        }

        MainRecord record;

        record.m_Date=QDate::fromString(baseName.mid(0,8),"yyyyMMdd").toString("yyyy-MM-dd");
        record.m_No=baseName;
        record.m_Brand=name;

       // record.m_NumNg[0]=ngNum;
        memcpy(record.m_NumNg,ngNum,sizeof (int)*CAMERANUM_MAX);
        QString sss=baseName.mid(8,-1);
        record.m_Shift=baseName.mid(8,-1).toInt();
        record.m_StrShift=record.m_Shift==0?"早班":(record.m_Shift==1?"中班":"晚班");
        record.m_NumAll=allNum;
        lst.push_back(record);
    }

}



QString DsFileRecord::getCurMainRecordNo()
{
    QDate date(QDate::currentDate());
    return date.toString("yyyyMMdd")+ToStr33(static_cast<int>(DSSystemParam::getCurrentShift()),1);
}

void DsFileRecord::updateRecord()
{
    QString fileName=DSSystemParam::AppPath+"/record/"+getCurMainRecordNo()+".rec";
    FileHelper::makeDir(DSSystemParam::AppPath+"/record");
    QFileInfo fi(fileName);

    PackageChecker* pc=PackageChecker::getInstance();

    int tmpNgArr[CAMERANUM_MAX]={0};
    memcpy(tmpNgArr,pc->RunParam_CalcNumNgCams,sizeof (int)*CAMERANUM_MAX);
    saveRecord(fileName.toStdString(),
               DSSystemParam::BrandName.toStdString(),
               pc->RunParam_CalcNumAll,
               tmpNgArr,
               pc->Cameras.size(),
               128,
               PackageChecker::ErrRecord
               );


}

void DsFileRecord::loadCurRecord()
{
    QString fileName=DSSystemParam::AppPath+"/record/"+getCurMainRecordNo()+".rec";
    FileHelper::makeDir(DSSystemParam::AppPath+"/record");
    QFileInfo fi(fileName);
    DSDEBUG<<"filename:"<<fileName;

    fstream infile(fileName.toStdString(), ios::in);
    string lineStr;
    getline(infile, lineStr);
    QString qlinestr=QString::fromStdString(lineStr);
    if(!qlinestr.contains("product"))
    {
       // DSDEBUG<<QString("record文件中 %1 文件损坏!").arg(QString::fromStdString(fileName));
        return ;
    }


    if(!fi.exists()) return;
    int allNum=0,ngNum=0,cameraNum=0;
    char name[128]={0};
    getRecord(fileName.toStdString(),name,&allNum,&ngNum,&cameraNum,PackageChecker::ErrRecord);
//    PackageChecker::getInstance()->RunParam_CalcNumAll=allNum;
//    PackageChecker::getInstance()->RunParam_CalcNumNg=ngNum;
}

void DsFileRecord::loadRecord(QString fileName,int& cameraNum,int& allNum,int&ngNum,int record[16][128])
{
    char name[256]={0};

    getRecord(fileName.toStdString(),name,&allNum,&ngNum,&cameraNum,record);

}

void DsFileRecord::deleteByDate(int days)
{
    QVector<QString> files;
    FileHelper::getAllFilesInFolder(QString(DSSystemParam::AppPath+"/record/"),files);

    for(int idx=0;idx<files.size();++idx)
    {
        QFileInfo fi(QString(DSSystemParam::AppPath+"/record/"+files[idx]));
        QString baseName= fi.baseName();
        if(baseName.length()!=9||baseName==getCurMainRecordNo()) continue;

        QDate date=QDate::fromString(baseName.mid(0,8),"yyyyMMdd");

        if(date.addDays(days)<QDate::currentDate())
        {

           if(fi.isFile())
           {

               QFile::remove(fi.filePath());
           }

        }





    }

}


