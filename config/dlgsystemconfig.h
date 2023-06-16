#ifndef DLGSYSTEMCONFIG_H
#define DLGSYSTEMCONFIG_H

#include <QDialog>

#include <QPainter>
#include <QProxyStyle>
#include<QButtonGroup>
#include<QStyleOptionTab>
#include"params/dssystemparam.h"
namespace Ui {
class DlgSystemConfig;
}


class CustomTabStyle : public QProxyStyle
{
public:
    QSize sizeFromContents(ContentsType type, const QStyleOption *option, const QSize &size, const QWidget *widget) const
    {
        QSize s = QProxyStyle::sizeFromContents(type, option, size, widget);
        if (type == QStyle::CT_TabBarTab)
        {
            s.transpose();
            s.rwidth() = 160;             // 设置每个tabBar中item的大小
            s.rheight() = 40;
        }
        return s;
    }
    void drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
    {
        if (element == CE_TabBarTabLabel)
        {
            if (const QStyleOptionTab *tab = qstyleoption_cast<const QStyleOptionTab *>(option))
            {
                QRect allRect = tab->rect;
                //                allRect.setWidth(allRect.width() - 5);
                //                allRect.setHeight(allRect.height() - 2);
                //选中状态
                if (tab->state & QStyle::State_Selected)
                {
                    //save用以保护坐标，restore用来退出状态
                    painter->save();
                    painter->setBrush(QBrush(0x4D4D4D));
                    //矩形
                    //painter->drawRect(allRect.adjusted(0, 0, 10, 10));
                    painter->setPen(QPen(QColor(0x4d4d4d)));
                    painter->drawRect(allRect);
                    painter->restore();
                }
                //hover状态
                else if(tab->state & QStyle::State_MouseOver)
                {
                    painter->save();
                    painter->setBrush(QBrush(0x969696));
                    painter->setPen(QPen(QColor(0x969696)));
                    //painter->drawRoundedRect(tab->rect, 8, 8);
                    //painter->drawRect(allRect.adjusted(0, 0, 10, 10));
                    painter->drawRect(allRect);
                    painter->restore();
                }
                else
                {
                    painter->save();
                    painter->setBrush(QBrush(0xE6E6E6));
                    painter->setPen(QPen(QColor(0xE6E6E6)));
                    //painter->drawRoundedRect(tab->rect 8, 8);
                    //painter->drawRect(allRect.adjusted(0, 0, 10, 10));
                    painter->drawRect(allRect);
                    painter->restore();
                }
                QTextOption option;
                option.setAlignment(Qt::AlignCenter);
                painter->setFont(QFont("微软雅黑", 10, QFont::Medium));
                //painter->setPen(0xffffff);
                painter->drawText(allRect, tab->text, option);
                return;
            }
        }
        if (element == CE_TabBarTab)
        {
            QProxyStyle::drawControl(element, option, painter, widget);

        }
    }
};

class DlgSystemConfig : public QDialog
{
    Q_OBJECT

public:
    explicit DlgSystemConfig(QWidget *parent = nullptr);
    ~DlgSystemConfig();
private:
    void initControl();
private slots:
    void on_pushButtonOpen_clicked();
    void buttonSaveTypeJudge(int type);


    void on_pushButtonCancel_clicked();
    void on_pushButtonSaveRet_clicked();
    void on_pushButtonSave_clicked();

    void buttonOutputTypeJudge(int type);


    void buttonDisplayTypeJudge(int type);

    void buttonCheckModeJudge(int);
    void buttonEphDisplayJudge(int);






private:

    void InitStyle();
    QPoint mousePoint;
    bool mousePressed;
    bool max;
    QRect location;
private slots:
//    void on_btnMenu_Max_clicked();
//    void on_btnMenu_Min_clicked();
    void on_btnMenu_Close_clicked();


void buttonJudge(int idx);





    void on_pushButtonSaveRet1_clicked();

    
    void on_pushButtonCancel1_clicked();

    void on_pushButtonSave1_clicked();

    void on_pushButtonClearData_clicked();



    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();



    void on_pushButton_1_clicked();

    void on_pushButton_7_clicked();
    void on_pushButton_8_clicked();


    void on_checkBoxNGNum_clicked();

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
    void mouseMoveEvent(QMouseEvent *e)override;
    void mousePressEvent(QMouseEvent *e)override;
    void mouseReleaseEvent(QMouseEvent *)override;

private:
    Ui::DlgSystemConfig *ui;
    ENUMIMGSAVETYPE m_ImgSaveType;
    ENUMOUTPUTTYPE m_OutputType;
    ENUMDISPLAYTYPE m_DisplayType;

    ENUMCHECKMODETYPE m_CheckModeType;
    ENUMEPHDISPLAYTYPE m_EphDispType;





    QButtonGroup * m_BtnGroup;

};





#endif // DLGSYSTEMCONFIG_H
