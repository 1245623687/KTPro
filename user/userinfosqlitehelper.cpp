#include "userinfosqlitehelper.h"

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include"params/dssystemparam.h"

#include"dsdebug.h"
UserInfoSqliteHelper::UserInfoSqliteHelper()
{

}

void UserInfoSqliteHelper::getAllUserInfo(QVector<UserInfo> & lstuseinfo)
{
    QVector<UserInfo> pNullVector;
    lstuseinfo.swap(pNullVector);

    QSqlDatabase  database = QSqlDatabase::addDatabase("QSQLITE");
    QString basestr=DSSystemParam::AppPath+"/"+"UserManage.db";
    database.setDatabaseName(DSSystemParam::AppPath+"/"+"UserManage.db");
    bool ret= database.open();

    QSqlQuery sql_query(database);
    //sql_query.exec("select UserInfo.*, shift.name from UserInfo left outer join group on UserGroup.groupid = UserInfo.group order by group DESC" );
    sql_query.exec("select UserInfo.* from UserInfo where isdeleted=0" );

    //sql_query.exec("select UserInfo.* from UserInfo where isdelete='false'" );

    if(!sql_query.exec())
    {
        DSDEBUG<<sql_query.lastError();
        database.close();
        return ;
    }
    while(sql_query.next())
    {
        UserInfo ui;
        ui.setUserNo(sql_query.value(0).toInt());
        ui.setUserName(sql_query.value(1).toString());
        if(ui.UserName()=="superman")continue;
        ui.setPassword(sql_query.value(2).toString());
        ui.setUserGroup(static_cast<ENUMUSERGROUP>(sql_query.value(3).toInt()));

        lstuseinfo.push_back(ui);
    }

    database.close();
}

void UserInfoSqliteHelper::delUserbyName(QString uname)
{
    QSqlDatabase  database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName(DSSystemParam::AppPath+"/"+"UserManage.db");
    bool b=  database.open();
    QSqlQuery sql_query(database);

    sql_query.exec(QString("delete from UserInfo where username='%1'").arg(uname));
    if(!sql_query.exec())
    {
        DSDEBUG<<sql_query.lastError();
        database.close();
        return ;
    }
    database.close();
}


void UserInfoSqliteHelper::addUser(UserInfo& ui)
{

    QSqlDatabase  database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName(DSSystemParam::AppPath+"/"+"UserManage.db");
    database.open();
    QSqlQuery sql_query(database);

    QString sql=QString("insert into UserInfo (username,password,groupid) values('%1','%2',%3)").arg(ui.UserName()).arg(ui.Password()).arg(ui.UserGroup());

    if(!sql_query.exec(sql))
    {
        DSDEBUG<<sql_query.lastError();
        database.close();
        return ;
    }
    database.close();

}

void UserInfoSqliteHelper::updateUserByName(UserInfo& ui)
{

    QSqlDatabase  database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName(DSSystemParam::AppPath+"/"+"UserManage.db");
    database.open();
    QSqlQuery sql_query(database);

    QString sql=QString("update UserInfo set password='%1', groupid=%2 where username='%3'").arg(ui.Password()).arg(ui.UserGroup()).arg(ui.UserName());

    if(!sql_query.exec(sql))
    {
        DSDEBUG<<sql_query.lastError();
        database.close();
        return ;
    }
    database.close();
}

