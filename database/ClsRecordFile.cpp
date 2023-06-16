#include "ClsRecordFile.h"
#include <vector>
#include"dsdebug.h"
#include<QFile>

std::vector<std::string> splitWithStl(const std::string &str, const std::string &pattern)
{
    std::vector<std::string> resVec;

    if ("" == str)
    {
        return resVec;
    }
    //方便截取最后一段数据
    std::string strs = str + pattern;

    size_t pos = strs.find(pattern);
    size_t size = strs.size();

    while (pos != std::string::npos)
    {
        std::string x = strs.substr(0, pos);
        resVec.push_back(x);
        strs = strs.substr(pos + 1, size);
        pos = strs.find(pattern);
    }
    return resVec;
}



void saveRecord(string fileName,string productName, int allNum, int ngNum[CAMERANUM_MAX], int cameraNum, int maxChannel, int data[16][128])
{
    fstream outfile( fileName, fstream::out);
    outfile.clear();
    if (!outfile.is_open()) return;



    outfile << "product:" << productName << endl;
    outfile << "All:" << allNum << endl;

    outfile<<"Ng:";
    for(int i=0;i<CAMERANUM_MAX;i++)
    {
        outfile <<std::to_string(ngNum[i])<<" ";
    }
    outfile<<endl;


    //DSDEBUG<<"camera Num:"<<cameraNum;
    for (int cameraNo=0;cameraNo<cameraNum;++cameraNo)
    {
        //DSDEBUG<<"camera NO:"<<cameraNo;
        for (int channel=0;channel<maxChannel;++channel)
        {
            //DSDEBUG<<data[cameraNo][channel];
            outfile <<std::to_string(data[cameraNo][channel])<<" ";
        }

        outfile << endl;
    }
    outfile << "end";
    outfile.close();

}


void getRecord(string fileName, char* productName, int* allNum, int* ngNum, int *cameraNum, int data[16][128])
{
    fstream infile(fileName,ios::in);
    if (!infile.is_open()) return;
    string lineStr;

    getline(infile, lineStr);
    QString qlinestr=QString::fromStdString(lineStr);
    if(!qlinestr.contains("product"))
    {
        DSDEBUG<<QString("record文件中 %1 文件损坏!").arg(QString::fromStdString(fileName));
        return ;
    }

    if(lineStr.size()<=8)
    {
        infile.close();
        QFile fileTemp(QString::fromStdString(fileName));
        fileTemp.remove();
        return;
    }
    string tmp = lineStr.substr(8, lineStr.size() - 8);
    tmp.copy(productName, tmp.length(), 0);

    getline(infile, lineStr);
    if(lineStr.size()<=4)
    {
        infile.close();
        QFile fileTemp(QString::fromStdString(fileName));
        fileTemp.remove();
        return;
    }
    tmp=lineStr.substr(4, lineStr.size() -4);
    *allNum = atoi(tmp.c_str());

    getline(infile, lineStr);
    if(lineStr.size()<=3)
    {
        infile.close();
        QFile fileTemp(QString::fromStdString(fileName));
        fileTemp.remove();
        return;
    }
    tmp = lineStr.substr(3, lineStr.size() -3);
    *ngNum = atoi(tmp.c_str());
    int idx = 0;
    while (getline(infile,lineStr))
    {
        if(lineStr=="end") break;
        std::vector<std::string> vStr;

        vStr=splitWithStl(lineStr, " ");
        if(vStr.size())
        {
            vStr.pop_back();
        }
        for (int i_idx=0;i_idx<vStr.size();++i_idx)
        {
            data[idx][i_idx] = atoi(vStr[i_idx].c_str());
        }
        idx++;
    }
    *cameraNum = idx;
    infile.close();
}


int getSimpleRecord(string fileName, char* productName, int* allNum, int* ngNum)
{
    fstream infile(fileName, ios::in);
    if (!infile.is_open()) return -1;
    string lineStr;

    getline(infile, lineStr);
    QString qlinestr=QString::fromStdString(lineStr);
    if(!qlinestr.contains("product"))
    {
        DSDEBUG<<QString("record文件中 %1 文件损坏!").arg(QString::fromStdString(fileName));
        return -1;
    }
    string tmp = lineStr.substr(8, lineStr.size() - 8);
    tmp.copy(productName, tmp.length(), 0);

    getline(infile, lineStr);
    tmp = lineStr.substr(4, lineStr.size() - 4);

    QString Tem(tmp.c_str());
    QString ret= Tem.remove(QChar(','),Qt::CaseInsensitive);
    *allNum = atoi(ret.toStdString().c_str());

    int ngNumArr[CAMERANUM_MAX]={0};
    getline(infile, lineStr);
    tmp = lineStr.substr(3, lineStr.size() - 3);

    std::vector<std::string> vStr;
    vStr=splitWithStl(tmp, " ");
    for(int i=0;i<CAMERANUM_MAX;i++)
    {
        QString Tem(vStr[i].c_str());
        QString ret= Tem.remove(QChar(','),Qt::CaseInsensitive);
        ngNumArr[i]=atoi(ret.toStdString().c_str());
    }

    memcpy(ngNum,ngNumArr,sizeof (int)*CAMERANUM_MAX);
   // *ngNum = atoi(tmp.c_str());
    infile.close();
    return 0;
}
