#ifndef KTHREADSAVEIMAGE_H
#define KTHREADSAVEIMAGE_H
#include"kthreadbase.h"
#include<queue>
#include"dsdebug.h"
#include"CheckOperator/ImageBase.hpp"

class KThreadSaveImage : public KThreadBase
{
private:
    typedef struct StuSaveImg
    {
        QString path;
        OpencvImage Image;
    }StuSaveImg;

public:



    static KThreadSaveImage* getInstance()
    {
        m_Mutex.lock();
        if (m_Instance == nullptr)
        {
            m_Instance = new KThreadSaveImage();
        }
        m_Mutex.unlock();
        return m_Instance;
    }


    void run();

    void addImg(QString strPath,OpencvImage& image);

    ~KThreadSaveImage()
    {
        DSDEBUG<<" ~DsThreadSaveImage()"<<endl;
    }
private:
    void stop(){KThreadBase::stop();}
private:
    class GC
    {
    public :
        ~GC()
        {
            if (m_Instance != nullptr )
            {

                delete m_Instance;
                m_Instance = nullptr ;
            }
        }
    };
    static GC gc;


    static std::mutex m_Mutex;
    static KThreadSaveImage* m_Instance;
    KThreadSaveImage();
    std::queue<StuSaveImg> m_SaveImgs;

    std::mutex m_MutexSave;
};

#endif // DSTHREADSAVEIMAGE_H
