#ifndef MAINRECORD_H
#define MAINRECORD_H




#include<QString>
#include<QDate>
#include"dsbase.h"

class MainRecord
{
public:
    MainRecord();


public:
    QString m_No;
    QString m_Date;
    int m_Shift;
    QString m_StrShift;
    QString m_Brand;

    int m_NumAll;
    int m_NumNg[CAMERANUM_MAX];
};


class DefectRecord
{
public:
    DefectRecord(){}



public:
    int m_No;
    QString m_RecordNo;
    int m_CameraNo;
    int m_Channel;
    int m_ErrId;
    QString m_Datetime;
    QString m_ErrName;

};

class ErrType
{
public:
    int m_Id;
    QString m_Name;
};



#endif // MAINRECORD_H
