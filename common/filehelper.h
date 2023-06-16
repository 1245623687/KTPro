#ifndef FILEHELPER_H
#define FILEHELPER_H

#include<QString>
#include<QDir>
#include<QFileInfo>
#include<QVector>

class FileHelper
{
public:
    FileHelper();

    static int makeDir(QString fileName);

    //获取路径下面的所有文件夹名称
    static void getAllFileFolder(QString dirPath, QVector<QString> &folders);

    static void getAllFileFolderRegex(QString dirPath, QString regex,QVector<QString> &folders);

    static void getAllFilesInFolder(QString dirPath,QVector<QString> &files);
    static void getAllImgFilesInFolder(QString dirPath,QVector<QString> &files);

    static void createFile(QString filePath,QString fileName);


    static bool copyRecursively(const QString &srcFilePath,
                                const QString &tgtFilePath)
    {
        QFileInfo srcFileInfo(srcFilePath);
        if (srcFileInfo.isDir()) {
            QDir targetDir(tgtFilePath);

            //            if (!targetDir.mkdir(QFileInfo(tgtFilePath).fileName()))
            //                return false;
            if (!targetDir.mkpath(tgtFilePath))
                return false;
            QDir sourceDir(srcFilePath);
            QStringList fileNames = sourceDir.entryList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot | QDir::Hidden | QDir::System);
            foreach (const QString &fileName, fileNames) {
                const QString newSrcFilePath
                        = srcFilePath + QLatin1Char('/') + fileName;
                const QString newTgtFilePath
                        = tgtFilePath + QLatin1Char('/') + fileName;
                if (!copyRecursively(newSrcFilePath, newTgtFilePath))
                    return false;
            }
        } else {
            QFile ftgt(tgtFilePath);
            if(ftgt.exists())
            {
                ftgt.remove();
            }
            if (!QFile::copy(srcFilePath, tgtFilePath))
                return false;
        }
        return true;
    }

    //删除文件夹下所有文件不包含文件夹
    static void deleteDir(QString dirPath);

    //删除文件夹下所有文件包含文件夹
    static void deleteDirAll(QString dirPath);
};

#endif // FILEHELPER_H
