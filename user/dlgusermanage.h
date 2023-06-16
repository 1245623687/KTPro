#ifndef DLGUSERMANAGE_H
#define DLGUSERMANAGE_H

#include <QDialog>
#include"user/userinfosqlitehelper.h"
#include"kcontrols/klineedit.h"
#include<QStandardItemModel>
#include"kcontrols/klineeditkb.h"
#include"kcontrols/dlgkeyboard.h"
namespace Ui {
class DlgUserManage;
}

class DlgUserManage : public QDialog
{
    Q_OBJECT

    enum ENUMSATE
    {
        ENUMSATE_LOGIN=0,
        ENUMSATE_CHANGEPWD=1,
        ENUMSATE_ADDUSER=2,
        ENUMSATE_MANAGEUSER=3,
        ENUMSATE_CURUSER=4
    };

public:
    explicit DlgUserManage(QWidget *parent = nullptr);
    ~DlgUserManage();
    void InitControl();
private:

    void InitStyle();
    QPoint mousePoint;
    bool mousePressed;
    bool max;
    QRect location;
private slots:
    void showKeyBoard(KControlsBase* pLineEdit);
    void on_btnMenu_Close_clicked();
    void on_Changepwd_clicked(bool b);

    void on_comboBoxUserName_FocusIn() ;
    void on_comboBoxUserName_FocusOut() ;

    void on_toolButtonIcon_clicked();

    void on_toolButtonAddUser_clicked();

    void on_toolButtonadduserok_clicked();

    void on_toolButtonaddusercancel_clicked();

    void on_toolButtonManage_clicked();

    void on_toolButtonLogin_clicked();


    void on_toolButtonChangepwdOk_clicked();

    void on_textEditNames_selectionChanged();

    void on_tableView_clicked(const QModelIndex &index);

    void on_toolButtonCurUserManage_clicked();

    void on_toolButtonCurUserLogoff_clicked();

    void on_toolButtonCurAddUser_clicked();


protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
    void mouseMoveEvent(QMouseEvent *e)override;
    void mousePressEvent(QMouseEvent *e)override;
    void mouseReleaseEvent(QMouseEvent *)override;

private:
    QString getGroupName(ENUMUSERGROUP g)
    {
        switch (g)
        {
//        case   ENUMUSERGROUP_OPERATOR:
//            return "Operator";
//        case ENUMUSERGROUP_MANAGER:
//            return "Manager";
//        case ENUMUSERGROUP_ADMIN:
//            return "SuperAdmin";

        case   ENUMUSERGROUP_OPERATOR:
            return "操作员";
        case ENUMUSERGROUP_MANAGER:
            return "管理员";
        case ENUMUSERGROUP_ADMIN:
            return "超级管理员";
        }

    }
    void updateUserTableView();

    QStandardItemModel* m_UserModel;
private:
    Ui::DlgUserManage *ui;

private:
    ENUMSATE m_State=ENUMSATE_LOGIN;

    QVector<UserInfo> m_lstUserInfo;

};

#endif // DLGUSERMANAGE_H
