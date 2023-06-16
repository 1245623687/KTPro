#ifndef USERINFO_H
#define USERINFO_H

#include<QString>

enum ENUMUSERGROUP
{
    ENUMUSERGROUP_OPERATOR=2,
    ENUMUSERGROUP_MANAGER=1,
    ENUMUSERGROUP_ADMIN=0
};


class UserInfo
{
public:
    UserInfo(){}

    UserInfo(int no,QString name,QString pwd,ENUMUSERGROUP group):m_UserNo(no),m_UserName(name),m_Password(pwd),m_UserGroup(group)
    {}


    ENUMUSERGROUP UserGroup(){return m_UserGroup;}
    void setUserGroup(ENUMUSERGROUP group)
    {
        m_UserGroup=group;
    }

    QString UserName(){return m_UserName;}
    void setUserName(QString name)
    {
        m_UserName=name;
    }

    QString Password(){return m_Password;}
    void setPassword(QString pwd)
    {
        m_Password=pwd;
    }

    int UserNo(){return m_UserNo;}
    void setUserNo(int no)
    {
        m_UserNo=no;
    }


public:
    ENUMUSERGROUP m_UserGroup;
    QString m_UserName;
    QString m_Password;
    int m_UserNo;
    bool Isnew=false;
    bool IsChanged=false;
    bool IsDelete=false;
};

#endif // USERINFO_H
