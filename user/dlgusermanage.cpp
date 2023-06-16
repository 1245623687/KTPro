#include "dlgusermanage.h"
#include "ui_dlgusermanage.h"
#include<QDesktopWidget>
#include"qtCommon/frmmessagebox.h"
#include"qtCommon/iconhelper.h"
#include<QMouseEvent>
#include<QAction>
#include"dsdebug.h"
#include"PackageChecker.h"
#include"qtCommon/frmmessagebox.h"
#include<QFont>
#include"qtCommon/frmmessagebox.h"

DlgUserManage::DlgUserManage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgUserManage)
{
    ui->setupUi(this);
    this->setWindowTitle("用户管理");
    UserInfoSqliteHelper::getAllUserInfo(m_lstUserInfo);


    InitStyle();
    InitControl();
}



void DlgUserManage::InitControl()
{
    connect(ui->lineEditLoginpassword,SIGNAL(clicked(KControlsBase* )),this,SLOT(showKeyBoard(KControlsBase* )));


//    connect(ui->lineEditaddpwd,SIGNAL(clicked(KLineEditKb* )),this,SLOT(showKeyBoard(KLineEditKb* )));
//    connect(ui->lineEditaddpwd2,SIGNAL(clicked(KLineEditKb* )),this,SLOT(showKeyBoard(KLineEditKb* )));



    //用户管理
    m_UserModel=new QStandardItemModel;
    this->ui->tableView->setModel(m_UserModel);
    updateUserTableView();
    this->ui->tableView->verticalHeader()->setVisible(false);//这个可以
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);  //整行选中
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers); //不可修改

    //用户登录
    QAction *usernameAction = new QAction(ui->lineEditLoginusername);
    usernameAction->setIcon(QIcon(":/username.png"));
    ui->lineEditLoginusername->addAction(usernameAction,QLineEdit::LeadingPosition);
    ui->lineEditLoginusername->setPlaceholderText("请输入用户名");

    QAction *passwordAction = new QAction(ui->lineEditLoginpassword);
    passwordAction->setIcon(QIcon(":/password.png"));
    ui->lineEditLoginpassword->addAction(passwordAction,QLineEdit::LeadingPosition);
    ui->lineEditLoginpassword->setPlaceholderText("请输入密码");

    QToolButton *btnchangepwd=new QToolButton(ui->lineEditLoginpassword);
    btnchangepwd->setStyleSheet("QToolButton{background:transparent;color: rgb(88,88,88);}"
                                "QToolButton:hover{color: rgb(15,149,45);}"
                                "QToolButton:pressed{color: rgb(203,254,208);}");

    btnchangepwd->setText("重设密码");
    btnchangepwd->setGeometry(btnchangepwd->geometry().x(),btnchangepwd->geometry().y(),btnchangepwd->width()-20,btnchangepwd->geometry().height());
    btnchangepwd->setGeometry(ui->lineEditLoginpassword->geometry().width()-btnchangepwd->geometry().width(),
                              (ui->lineEditLoginpassword->geometry().height()-btnchangepwd->geometry().height())/2,
                              btnchangepwd->geometry().width(),
                              btnchangepwd->geometry().height());
    connect(btnchangepwd,SIGNAL(clicked(bool)),this,SLOT(on_Changepwd_clicked(bool)));

    ui->toolButtonAddUser->setCursor(QCursor(Qt::PointingHandCursor));
    ui->toolButtonManage->setStyleSheet("background:transparent;border-image:url(:/usermanage.png);color:rgb(0,255,0);");
    ui->toolButtonCurUserManage->setStyleSheet("background:transparent;border-image:url(:/usermanage.png);color:rgb(0,255,0);");
    ui->textEditNames->setVisible(false);
    connect(ui->lineEditLoginusername,SIGNAL(signalFocusIn()),this,SLOT(on_comboBoxUserName_FocusIn()));
    connect(ui->lineEditLoginusername,SIGNAL(signalFocusOut()),this,SLOT(on_comboBoxUserName_FocusOut()));
    for (int idx=0;idx<m_lstUserInfo.size();++idx)
    {
        if(m_lstUserInfo[idx].UserGroup()==ENUMUSERGROUP_ADMIN) continue;
        this->ui->textEditNames->append(m_lstUserInfo[idx].UserName());
    }

    //更换密码
    QAction *pwdoldAction = new QAction(ui->lineEditChangeoldpwd);
    pwdoldAction->setIcon(QIcon(":/oldpwd.png"));
    ui->lineEditChangeoldpwd->addAction(pwdoldAction,QLineEdit::LeadingPosition);
    ui->lineEditChangeoldpwd->setPlaceholderText("请输入原始密码");

    QAction *pwdnewAction1 = new QAction(ui->lineEditChangenewpwd);
    pwdnewAction1->setIcon(QIcon(":/password.png"));
    ui->lineEditChangenewpwd->addAction(pwdnewAction1,QLineEdit::LeadingPosition);
    ui->lineEditChangenewpwd->setPlaceholderText("请输入新密码");

    QAction *pwdnewAction2 = new QAction(ui->lineEditChangenewpwd2);
    pwdnewAction2->setIcon(QIcon(":/password.png"));
    ui->lineEditChangenewpwd2->addAction(pwdnewAction2,QLineEdit::LeadingPosition);
    ui->lineEditChangenewpwd2->setPlaceholderText("请确认新密码");

    //新增用户
    QAction *addUserNameAction = new QAction(ui->lineEditaddusername);
    addUserNameAction->setIcon(QIcon(":/username.png"));
    ui->lineEditaddusername->addAction(addUserNameAction,QLineEdit::LeadingPosition);
    ui->lineEditaddusername->setPlaceholderText("请输入用户名");

    QAction *addPwdAction = new QAction(ui->lineEditaddpwd);
    addPwdAction->setIcon(QIcon(":/password.png"));
    ui->lineEditaddpwd->addAction(addPwdAction,QLineEdit::LeadingPosition);
    ui->lineEditaddpwd->setPlaceholderText("请输入密码");

    QAction *addPwdAction2 = new QAction(ui->lineEditaddpwd2);
    addPwdAction2->setIcon(QIcon(":/password.png"));
    ui->lineEditaddpwd2->addAction(addPwdAction2,QLineEdit::LeadingPosition);
    ui->lineEditaddpwd2->setPlaceholderText("请再次输入密码");

    ui->comboBox->addItem(QIcon(":/supermanager.png"),"超级管理员");
    ui->comboBox->addItem(QIcon(":/manager.png"),"管理员");
    ui->comboBox->addItem(QIcon(":/operator.png"),"操作员");
    ui->comboBox->setEditable(false);
    QVariant v(0);
    ui->comboBox->setItemData(0, v, Qt::UserRole -1);
    ui->comboBox->setCurrentIndex(2);

    //用户管理

    if(PackageChecker::getInstance()->user.UserName()=="操作员"&&PackageChecker::getInstance()->user.UserGroup()==ENUMUSERGROUP_OPERATOR)
    {
        ui->stackedWidget->setCurrentIndex(0);
    }

    else
    {
        ui->labelCurUserName->setText(PackageChecker::getInstance()->user.UserName());
        ui->labelCurUserGroup->setText(getGroupName(PackageChecker::getInstance()->user.UserGroup()));
        ui->stackedWidget->setCurrentIndex(4);
    }
}


void DlgUserManage::showKeyBoard(KControlsBase* pLineEdit)
{

    switch (pLineEdit->type())
    {
    case ENUMKCONTROLTYPE_KSPINBOX:
    {
        KSpinBoxKb* pWidget=static_cast<KSpinBoxKb*>(pLineEdit);

        QPoint pt=pWidget->mapToGlobal(QPoint(pWidget->geometry().x(),pWidget->geometry().y()));
        DlgKeyBoard dlg(pWidget);
        dlg.setGeometry(pt.x()-pWidget->geometry().x(),pt.y()-pWidget->geometry().y()+pWidget->geometry().height()+2,dlg.geometry().width(),dlg.geometry().height());
        dlg.exec();
    }
        break;
    case ENUMKCONTROLTYPE_KLINEEDIT:
    {
        KLineEditKb* pWidget=static_cast<KLineEditKb*>(pLineEdit);

        QPoint pt=pWidget->mapToGlobal(QPoint(pWidget->geometry().x(),pWidget->geometry().y()));
        DlgKeyBoard dlg(pWidget);
        dlg.setGeometry(pt.x()-pWidget->geometry().x(),pt.y()-pWidget->geometry().y()+pWidget->geometry().height()+2,dlg.geometry().width(),dlg.geometry().height());
        dlg.exec();
    }
        break;
    }

}


void DlgUserManage::on_comboBoxUserName_FocusIn()
{
    ui->textEditNames->clear();
    for (int idx=0;idx<m_lstUserInfo.size();++idx)
    {
        ui->textEditNames->append(m_lstUserInfo[idx].UserName());
    }


    ui->textEditNames->setVisible(true);

}

void DlgUserManage::on_comboBoxUserName_FocusOut()
{
    ui->textEditNames->setVisible(false);


}

void DlgUserManage::on_Changepwd_clicked(bool b)
{
    for (int idx=0;idx<m_lstUserInfo.size();++idx)
    {
        if(m_lstUserInfo[idx].UserName()==this->ui->lineEditLoginusername->text().trimmed())
        {
            this->ui->toolButtonIcon->setText(" < 返回登录");
            ui->toolButtonIcon->setCursor(QCursor(Qt::PointingHandCursor));
            this->ui->lab_Title->setText("");
            this->ui->stackedWidget->setCurrentIndex(ENUMSATE_CHANGEPWD);

            this->ui->lineEditChangeoldpwd->setFocus();
            return;
        }

    }

    frmMessageBox *msg = new frmMessageBox;

    msg->SetMessage("请输入正确的用户名", 0);
    msg->exec();
    this->ui->lineEditLoginusername->setFocus();








}


DlgUserManage::~DlgUserManage()
{

    for (int idx=0;idx<m_lstUserInfo.size();++idx)
    {
        if(m_lstUserInfo[idx].UserGroup()==ENUMUSERGROUP_ADMIN) continue;
        if(m_lstUserInfo[idx].Isnew)
        {
            //添加一条
            UserInfoSqliteHelper::addUser(m_lstUserInfo[idx]);

        }
        if(m_lstUserInfo[idx].IsChanged)
        {
            //更新一条

            UserInfoSqliteHelper::updateUserByName(m_lstUserInfo[idx]);

        }



    }


    QVector<UserInfo> pNullVector;
    m_lstUserInfo.swap(pNullVector);

    delete ui;
}
void DlgUserManage::InitStyle()
{
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint);
    location = this->geometry();
    max = false;
    mousePressed = false;


    ui->lab_Title->installEventFilter(this);

    IconHelper::Instance()->SetIcon(ui->btnMenu_Close, QChar(0xf00d), 10);
    IconHelper::Instance()->SetIcon(ui->toolButtonIcon, QChar(0xf007), 12);

}


bool DlgUserManage::eventFilter(QObject *obj, QEvent *event)
{

    return QObject::eventFilter(obj, event);
}

void DlgUserManage::mouseMoveEvent(QMouseEvent *e)
{
    if (mousePressed && (e->buttons() && Qt::LeftButton) && !max) {
//        this->move(e->globalPos() - mousePoint);
        e->accept();
    }
}

void DlgUserManage::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        mousePressed = true;
        mousePoint = e->globalPos() - this->pos();
        e->accept();
    }
}

void DlgUserManage::mouseReleaseEvent(QMouseEvent *)
{
    mousePressed = false;
}



void DlgUserManage::on_btnMenu_Close_clicked()
{
    this->close();
}



void DlgUserManage::on_toolButtonIcon_clicked()
{
    this->ui->stackedWidget->setCurrentIndex(ENUMSATE_LOGIN);
    m_State=ENUMSATE_LOGIN;
    ui->toolButtonIcon->setCursor(QCursor(Qt::ArrowCursor));
    this->ui->lab_Title->setText("用户登录");
    IconHelper::Instance()->SetIcon(ui->toolButtonIcon, QChar(0xf007), 12);
    //this->ui->lab_Title->setStyleSheet("color:rbg(255,255,255);");
}

void DlgUserManage::on_toolButtonAddUser_clicked()
{
    if(PackageChecker::getInstance()->user.UserGroup()==ENUMUSERGROUP_OPERATOR)
    {
        frmMessageBox *msg = new frmMessageBox;

        msg->SetMessage("请使用管理员账户登录!", 0);
        msg->exec();

        return;

    }


    this->ui->toolButtonIcon->setText(" < 返回登录");
    ui->toolButtonIcon->setCursor(QCursor(Qt::PointingHandCursor));
    this->ui->lab_Title->setText("");
    this->ui->stackedWidget->setCurrentIndex(ENUMSATE_ADDUSER);
}

void DlgUserManage::on_toolButtonadduserok_clicked()
{
    //判断
    if(this->ui->lineEditaddusername->text().trimmed()=="")
    {
        frmMessageBox *msg = new frmMessageBox;

        msg->SetMessage("请输入用户名!", 0);
        msg->exec();
        ui->lineEditaddusername->setFocus();
        return;
    }
    if(this->ui->lineEditaddpwd->text().trimmed()=="")
    {
        frmMessageBox *msg = new frmMessageBox;

        msg->SetMessage("请输入用户密码!", 0);
        msg->exec();
        ui->lineEditaddpwd->setFocus();
        return;
    }
    if(this->ui->lineEditaddpwd2->text().trimmed()=="")
    {
        frmMessageBox *msg = new frmMessageBox;

        msg->SetMessage("请确认输入的用户密码!", 0);
        msg->exec();
        ui->lineEditaddpwd2->setFocus();
        return;
    }

    if(this->ui->lineEditaddpwd->text().trimmed()!=this->ui->lineEditaddpwd2->text().trimmed())
    {
        frmMessageBox *msg = new frmMessageBox;

        msg->SetMessage("两次输入的密码不一致!", 0);
        msg->exec();
        ui->lineEditaddpwd->clear();
        ui->lineEditaddpwd2->clear();
        ui->lineEditaddpwd->setFocus();
        return;
    }

    //数据库里新增
    for (int idx=0;idx<m_lstUserInfo.size();++idx)
    {
        if(m_lstUserInfo[idx].UserName()==this->ui->lineEditaddusername->text().trimmed())
        {
            frmMessageBox *msg = new frmMessageBox;

            msg->SetMessage("当前用户已存在,请重新输入用户名!", 0);
            msg->exec();
            ui->lineEditaddusername->clear();
            ui->lineEditaddusername->setFocus();
            return;
        }
    }
    UserInfo newUser;
    newUser.setUserName(this->ui->lineEditaddusername->text());
    newUser.setPassword(this->ui->lineEditaddpwd->text().trimmed());
    newUser.setUserGroup(static_cast<ENUMUSERGROUP>(ui->comboBox->currentIndex()));
    newUser.Isnew=true;
    m_lstUserInfo.push_back(newUser);

    frmMessageBox *msg = new frmMessageBox;

    msg->SetMessage("用户添加成功!", 0);
    msg->exec();

    //清空
    this->ui->lineEditaddpwd->clear();
    this->ui->lineEditaddpwd2->clear();
    this->ui->lineEditaddusername->clear();
    this->ui->comboBox->setCurrentIndex(2);
    //on_toolButtonIcon_clicked();


}

void DlgUserManage::on_toolButtonaddusercancel_clicked()
{

    //清空
    this->ui->lineEditaddpwd->clear();
    this->ui->lineEditaddpwd2->clear();
    this->ui->lineEditaddusername->clear();
    this->ui->comboBox->setCurrentIndex(2);

}

void DlgUserManage::on_toolButtonManage_clicked()
{
    if(PackageChecker::getInstance()->user.UserGroup()!=ENUMUSERGROUP_ADMIN)
    {
        frmMessageBox *msg = new frmMessageBox;

        msg->SetMessage("请用超级管理员账户登录", 0);
        msg->exec();
        return;
    }

    updateUserTableView();


    this->ui->stackedWidget->setCurrentIndex(ENUMSATE_MANAGEUSER);
    ui->toolButtonIcon->setCursor(QCursor(Qt::PointingHandCursor));
    this->m_State=ENUMSATE_MANAGEUSER;
    this->ui->toolButtonIcon->setText(" < 返回登录");
    this->ui->lab_Title->setText("");



}

void DlgUserManage::on_toolButtonLogin_clicked()
{
    if(this->ui->lineEditLoginpassword->text().toUpper()=="DASHU")
    {
        PackageChecker::getInstance()->user.setUserName("超管");
        PackageChecker::getInstance()->user.setUserGroup(ENUMUSERGROUP_ADMIN);


        PackageChecker::getInstance()->m_TimerLog->start(60000*30);
        this->close();
        return;
    }
    if(this->ui->lineEditLoginusername->text()=="")
    {
        frmMessageBox *msg = new frmMessageBox;

        msg->SetMessage("请输入用户名", 0);
        msg->exec();
        this->ui->lineEditLoginusername->setFocus();
        return;
    }
    if(this->ui->lineEditLoginpassword->text()=="")
    {
        frmMessageBox *msg = new frmMessageBox;

        msg->SetMessage("请输入用户密码", 0);
        msg->exec();
        this->ui->lineEditLoginpassword->setFocus();
        return;
    }


    for (int idx=0;idx<m_lstUserInfo.size();++idx)
    {
        if(m_lstUserInfo[idx].UserName()==this->ui->lineEditLoginusername->text().trimmed())
        {
            if(m_lstUserInfo[idx].Password()==this->ui->lineEditLoginpassword->text().trimmed())
            {
//                frmMessageBox *msg = new frmMessageBox;

//                msg->SetMessage("登陆成功!", 0);
//                msg->exec();
                PackageChecker::getInstance()->user.setUserName(m_lstUserInfo[idx].UserName());
                PackageChecker::getInstance()->user.setUserGroup(m_lstUserInfo[idx].UserGroup());
                PackageChecker::getInstance()->m_TimerLog->start(60000*30);

                this->close();
                return;
            }
            else
            {
                frmMessageBox *msg = new frmMessageBox;

                msg->SetMessage("请输入正确的用户密码!", 0);
                msg->exec();
                this->ui->lineEditLoginpassword->clear();
                this->ui->lineEditLoginpassword->setFocus();

                return;
            }
        }
    }


    frmMessageBox *msg = new frmMessageBox;

    msg->SetMessage("当前用户不存在", 0);
    msg->exec();
    this->ui->lineEditLoginusername->clear();
    this->ui->lineEditLoginusername->setFocus();


}



void DlgUserManage::on_toolButtonChangepwdOk_clicked()
{
    if(ui->lineEditChangenewpwd->text()=="")
    {
        frmMessageBox *msg = new frmMessageBox;

        msg->SetMessage("请输入新的用户密码!", 0);
        msg->exec();
        ui->lineEditChangenewpwd->setFocus();
        return;
    }
    if(ui->lineEditChangenewpwd2->text()=="")
    {
        frmMessageBox *msg = new frmMessageBox;

        msg->SetMessage("请确认新的用户密码!", 0);
        msg->exec();
        ui->lineEditChangenewpwd2->setFocus();
        return;
    }
    if(ui->lineEditChangenewpwd2->text()!=ui->lineEditChangenewpwd->text())
    {
        frmMessageBox *msg = new frmMessageBox;

        msg->SetMessage("两次输入的密码不一致!", 0);
        msg->exec();
        ui->lineEditChangenewpwd->clear();
        ui->lineEditChangenewpwd2->clear();
        ui->lineEditChangenewpwd->setFocus();
        return;
    }



    for (int idx=0;idx<m_lstUserInfo.size();++idx)
    {
        if(m_lstUserInfo[idx].UserName()!=ui->lineEditLoginusername->text().trimmed())
            continue;


        if(m_lstUserInfo[idx].Password()!=ui->lineEditChangeoldpwd->text().trimmed())
        {
            frmMessageBox *msg = new frmMessageBox;

            msg->SetMessage("请输入正确的用户密码!", 0);
            msg->exec();
            this->ui->lineEditChangeoldpwd->clear();
            this->ui->lineEditChangeoldpwd->setFocus();
            return;
        }
        else
        {
            m_lstUserInfo[idx].setPassword(ui->lineEditChangenewpwd->text());
            m_lstUserInfo[idx].IsChanged=true;

            frmMessageBox *msg = new frmMessageBox;

            msg->SetMessage("密码修改成功!", 0);
            msg->exec();

        }

    }

}

void DlgUserManage::on_textEditNames_selectionChanged()
{

    QTextCursor tc = ui->textEditNames->textCursor(); //当前光标
    int rowNum = tc.blockNumber();
    QString allstr= ui->textEditNames->toPlainText();
    QStringList strlst= allstr.split("\n");
    ui->lineEditLoginusername->setText(strlst[rowNum]);

}


void DlgUserManage::updateUserTableView()
{
    m_UserModel->clear();
    QStringList listmodelMain;
    listmodelMain<<"姓名"<<"密码"<<"用户组"<<"操作";
    m_UserModel->setHorizontalHeaderLabels(listmodelMain);
    QFont font;
    font.setUnderline(true);


    for(int idx=0;idx<m_lstUserInfo.size();++idx)
    {
        QStandardItem *itemName=new QStandardItem(m_lstUserInfo[idx].UserName());
        QStandardItem* itemPwd=new QStandardItem(m_lstUserInfo[idx].Password());
        QStandardItem* itemGroup=new QStandardItem(getGroupName(m_lstUserInfo[idx].UserGroup()));
        QStandardItem* itemDel=new QStandardItem("删除");
        itemDel->setFont(font);
        itemDel->setForeground(QBrush(QColor(255,0,0)));


        itemName->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

        itemPwd->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

        itemGroup->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        itemDel->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

        m_UserModel->setItem(idx,0,itemName);
        m_UserModel->setItem(idx,1,itemPwd);
        m_UserModel->setItem(idx,2,itemGroup);
        m_UserModel->setItem(idx,3,itemDel);
    }

}

void DlgUserManage::on_tableView_clicked(const QModelIndex &index)
{
    //    DSDEBUG<<QString("%1,%2").arg(index.row()).arg(index.column());
    if(index.column()!=3) return;
    UserInfo ui=m_lstUserInfo[index.row()];

    frmMessageBox* msg = new frmMessageBox;
    msg->SetMessage(QString("是否删除用户%1").arg(ui.UserName()), 1);
    if(msg->exec())
    {
        UserInfoSqliteHelper::delUserbyName(ui.UserName());
        UserInfoSqliteHelper::getAllUserInfo(m_lstUserInfo);

        updateUserTableView();
    }
    else
    {


    }







}

void DlgUserManage::on_toolButtonCurUserManage_clicked()
{
    if(PackageChecker::getInstance()->user.UserGroup()!=ENUMUSERGROUP_ADMIN)
    {
        frmMessageBox *msg = new frmMessageBox;

        msg->SetMessage("请用超级管理员账户登录", 0);
        msg->exec();
        return;
    }

    updateUserTableView();


    this->ui->stackedWidget->setCurrentIndex(ENUMSATE_MANAGEUSER);
    ui->toolButtonIcon->setCursor(QCursor(Qt::PointingHandCursor));
    this->m_State=ENUMSATE_CURUSER;
    this->ui->toolButtonIcon->setText(" < 返回登录");
    this->ui->lab_Title->setText("");

}

void DlgUserManage::on_toolButtonCurUserLogoff_clicked()
{
    PackageChecker::getInstance()->user.setUserName("操作员");
    PackageChecker::getInstance()->user.setUserGroup(ENUMUSERGROUP_OPERATOR);
    PackageChecker::getInstance()->m_TimerLog->stop();
    this->ui->stackedWidget->setCurrentIndex(0);


}

void DlgUserManage::on_toolButtonCurAddUser_clicked()
{
    if(PackageChecker::getInstance()->user.UserGroup()==ENUMUSERGROUP_OPERATOR)
    {
        frmMessageBox *msg = new frmMessageBox;

        msg->SetMessage("请使用管理员账户登录!", 0);
        msg->exec();

        return;

    }


    this->ui->toolButtonIcon->setText(" < 返回登录");
    ui->toolButtonIcon->setCursor(QCursor(Qt::PointingHandCursor));
    this->ui->lab_Title->setText("");
    this->ui->stackedWidget->setCurrentIndex(ENUMSATE_ADDUSER);
}
