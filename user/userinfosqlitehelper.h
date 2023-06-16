#ifndef USERINFOSQLITEHELPER_H
#define USERINFOSQLITEHELPER_H
#include"userinfo.h"
#include<QVector>
class UserInfoSqliteHelper
{
public:
    static void getAllUserInfo(QVector<UserInfo> & lstuseinfo);
    static void delUserbyName(QString uname);
    static void addUser(UserInfo& ui);

    static void updateUserByName(UserInfo& ui);

private:
    UserInfoSqliteHelper();



};

#endif // USERINFOSQLITEHELPER_H
