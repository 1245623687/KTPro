

#ifndef DSFILERECORD_H
#define DSFILERECORD_H
#include"ClsRecordFile.h"
#include<QString>
#include"mainrecord.h"
#include<list>
class DsFileRecord
{
public:
    DsFileRecord();

    static QString getCurMainRecordNo();

    static void updateRecord();

    static void loadCurRecord();

    static void loadRecord(QString fileName,int& cameraNum,int& allNum,int&ngNum,int record[16][128]);

    static void loadMainRecord(QString dirPath,std::vector<MainRecord>& lst);

    static void getCurRecord(MainRecord& record);

    static void deleteByDate(int days);

};

#endif // DSFILERECORD_H


