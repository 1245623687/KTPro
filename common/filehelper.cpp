#include "filehelper.h"
#include"dsdebug.h"
#include<regex>
FileHelper::FileHelper()
{

}


int FileHelper::makeDir(QString fileName)
{
    if(QDir(fileName).exists()) return 0;
    if (!fileName.contains('.'))
    {
        QDir dir;
        dir.mkpath(fileName);
    }
    else
    {
        QFileInfo fi(fileName);
        QDir dir=fi.dir();
        dir.mkpath(dir.path());
    }
    return 0;

}

void FileHelper::getAllFileFolder(QString dirPath, QVector<QString> &folders)
{

    QDir dir(dirPath);
    dir.setFilter(QDir::Dirs);
    QFileInfoList lst=dir.entryInfoList();
    QFileInfoList::iterator it=lst.begin();
    for(;it!=lst.end();++it)
    {
        if((*it).fileName()=="."||(*it).fileName()=="..") continue;

        folders.push_back((*it).fileName());
    }

}



void FileHelper::getAllFileFolderRegex(QString dirPath,QString regex, QVector<QString> &folders)
{

    QDir dir(dirPath);
    dir.setFilter(QDir::Dirs);
    QFileInfoList lst=dir.entryInfoList();
    QFileInfoList::iterator it=lst.begin();

    std::regex regexcur{ regex.toStdString()+"_[123]" };


    for(;it!=lst.end();++it)
    {
        if((*it).fileName()=="."||(*it).fileName()=="..") continue;
        if(std::regex_match((*it).fileName().toStdString(),regexcur))
        {
            folders.push_back((*it).fileName());
        }
    }
}


void FileHelper::getAllImgFilesInFolder(QString dirPath,QVector<QString> &files)
{
    QDir dir(dirPath);

   // QString dirpa="C:/Users/Admin/Desktop/YK/KTOBACCOCHECKER/release/图像保存/薄壁嘴棒/NG/2022-06-06";
   // bool ret=dirpa==dirPath;
   // QDir dir(dirpa);
    if(!dir.exists()) return;
    QStringList filters;
    filters << QString("*.bmp");
    dir.setFilter(QDir::Files | QDir::NoSymLinks);
    dir.setNameFilters(filters);
    QFileInfoList lst=dir.entryInfoList();
    QFileInfoList::iterator it=lst.begin();
    for(;it!=lst.end();++it)
    {
        if((*it).fileName()=="."||(*it).fileName()=="..") continue;
        files.push_back((*it).fileName());
    }

}



void FileHelper::getAllFilesInFolder(QString dirPath,QVector<QString> &files)
{
    QDir dir(dirPath);
    QStringList filters;
    filters << QString("*.rec");
    dir.setFilter(QDir::Files | QDir::NoSymLinks);
    dir.setNameFilters(filters);
    QFileInfoList lst=dir.entryInfoList();
    QFileInfoList::iterator it=lst.begin();
    for(;it!=lst.end();++it)
    {
        if((*it).fileName()=="."||(*it).fileName()=="..") continue;
        files.push_back((*it).fileName());
    }

}

void FileHelper::createFile(QString filePath,QString fileName)
{
    QDir tempDir;
    QString currentDir = tempDir.currentPath();
    if(!tempDir.exists(filePath))
    {
        tempDir.mkpath(filePath);
    }
    QFile *tempFile = new QFile;
    tempDir.setCurrent(filePath);
    if(tempFile->exists(fileName))
    {
        return ;
    }
    tempFile->setFileName(fileName);
    if(!tempFile->open(QIODevice::WriteOnly|QIODevice::Text))
    {
        qDebug()<<"打开失败";
    }
    tempFile->close();
    tempDir.setCurrent(currentDir);
}

void FileHelper::deleteDir(QString dirPath)
{
    QDir dir(dirPath);
    dir.removeRecursively();
    makeDir(dirPath);
}
void FileHelper::deleteDirAll(QString dirPath)
{
    QDir dir(dirPath);
    dir.removeRecursively();

}

