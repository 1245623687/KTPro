#include "kthreadsaveimage.h"
#include<QFileInfo>
#include"dsdebug.h"
#include"common/filehelper.h"
#include "packagechecker.h"
#include <windows.h>
std::mutex KThreadSaveImage::m_Mutex;
KThreadSaveImage* KThreadSaveImage::m_Instance = nullptr;
KThreadSaveImage ::GC KThreadSaveImage ::gc;
KThreadSaveImage::KThreadSaveImage()
{

}

void KThreadSaveImage::run()
{
    DSDEBUG<<"thread id:"<<GetCurrentThreadId()<<endl;

    while(!isInterruptionRequested())
    {
        StuSaveImg tmp;
        bool hasImage=false;

        m_MutexSave.lock();
        if(m_SaveImgs.size())
        {
            //DSDEBUG<<"请求出列："<<m_SaveImgs.size()<<endl;
            tmp=this->m_SaveImgs.front();
            this->m_SaveImgs.pop();

//            if(PackageChecker::getInstance()->Options->getIsLimteMaxSize())
//            {
//                if(m_SaveImgs.size()>10)
//                {
//                    std::queue<StuSaveImg> tempSaveImgs;
//                    m_SaveImgs.swap(tempSaveImgs);
//                }
//            }
            hasImage=true;

        }


        m_MutexSave.unlock();
        if(hasImage)
        {
            QFileInfo fi(tmp.path);
            QString dirPath=fi.path();
            QDir dir(dirPath);
            if(!dir.exists(dirPath))
            {
                FileHelper::makeDir(dirPath);
            }

            //限制文件夹图片数量
//            if(PackageChecker::getInstance()->Options->getIsLimteMaxSize())
//            {
//                QVector<QString> files;
//                FileHelper::getAllImgFilesInFolder(dirPath ,files);
//                if(files.size()>=PackageChecker::getInstance()->Options->MaxSaveImgSize())
//                {
//                    //QFileInfo fi(dirPath+"/"+files[0]);

//                    for(int i=0;i<files.size()-PackageChecker::getInstance()->Options->MaxSaveImgSize();i++)
//                    {
//                        QFile fi(dirPath+"/"+files[i]);
//                        fi.remove();
//                    }
//                }
//            }
            tmp.Image.save(tmp.path);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}


void KThreadSaveImage::addImg(QString strPath, OpencvImage& image)
{

    //  DSDEBUG<<"请求入列："<<strPath<<endl;
    m_MutexSave.lock();
    StuSaveImg tmpStuSi;
    tmpStuSi.path=strPath;
    tmpStuSi.Image=image;
    m_SaveImgs.push(tmpStuSi);
    // DSDEBUG<<"入列完成"<<strPath<<"     m_SaveImgs.size:"<<m_SaveImgs.size()<<endl;
    m_MutexSave.unlock();
}
