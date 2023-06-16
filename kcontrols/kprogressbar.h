#ifndef KPROGRESSBAR_H
#define KPROGRESSBAR_H

#include<QApplication>
#include<QProgressDialog>
#include"dsdebug.h"
class KProgressBar
{
public:
    static void start(QString info)
    {
        if(progress==nullptr)
        {
            progress=new QProgressDialog;
            //DSDEBUG<<" progress=new QProgressDialog;"<<endl;
        }

        QApplication::setOverrideCursor(Qt::WaitCursor);//设置鼠标为等待状态

        progress->setWindowTitle("提示");
        progress->setLabelText(info);
        progress->setCancelButtonText(0);
        progress->setRange(0, 100);
        progress->setModal(true);
        progress->setFixedWidth(400);
        progress->setFixedHeight(65);
        progress->setWindowFlags(Qt::FramelessWindowHint);
//        progress->setStyleSheet("QProgressBar{border:0px solid #FFFFFF;"
//                                                       "height:30;"
//                                                       "background:rgb(105,105,105);"
//                                                       "text-align:center;"
//                                                       "color:rgb(255,255,0);"
  //                                                      "border-radius:5px;}"
//                                   "QProgressBar::chunk{"
//                                                       "border-radius:3px;"    // 斑马线圆角
//                                                       "border:0px "
//                                                       "solid black;" // 黑边，默认无边
//                                                       "background-color:rgb(0,255,0);}");

        progress->setStyleSheet(QLatin1String("QProgressBar { border: 2px solid grey; border-radius: 5px; background-color: #535353;}"
        "QProgressBar::chunk {background-color: #787878; width:20px;"
                                              "font: normal normal 30px Microsoft YaHei;}"
        "QProgressBar { border: 2px solid grey; border-radius: 5px; text-align: center;color: rgb(229, 229, 229);}"));

        progress->show();
    }
   static void setValue(int val)
    {
        progress->setValue(val);
        QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
        if(val==100)
        {
            QApplication::restoreOverrideCursor();
            progress->close();
            delete  progress;
            progress=nullptr;
        }
    }

private:
    KProgressBar();
    static QProgressDialog * progress;
    ~KProgressBar()
    {

    }
};

#endif // PROGRESSBAR_H
