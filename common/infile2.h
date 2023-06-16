#ifndef INFILE2_H
#define INFILE2_H

#include <QObject>
#include<QString>
#include<QSettings>
#include<QFile>
#include<QTextCodec>


class InFile2 : public QObject
{
    Q_OBJECT
public:
    InFile2(QObject *parent = nullptr);


    inline bool IsCreated(){return m_bCreated;}

    bool Save(QString filename);
    bool Save();
    bool Open(QString filename);
    bool GetValue(QString strSection,QString varName,QString &varvalue);
    bool SetValue(QString strSection,QString varName = "",QString varvalue = "",bool breplace = true);
//	bool ProcessLine(string strLine);
//	INIFile();
    InFile2(QString filename);
//	bool Create(string filename);

private:
    bool m_bCreated;
    QString m_initPath;

signals:

public slots:
};

#endif // INFILE2_H
