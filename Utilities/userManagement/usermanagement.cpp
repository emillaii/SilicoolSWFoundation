#include "usermanagement.h"

UserManagement::UserManagement() : dbHelper("qt_sql_userManagement_connection", "Users.db") {}

UserManagement::~UserManagement()
{
    if (isInit)
    {
        delete userModel;
    }
}

void UserManagement::init()
{
    if (!dbHelper.isTableExist("user"))
    {
        QSqlQuery createTable(dbHelper.db);
        createTable.prepare("create table user(name varchar(120) primary key, password varchar(120), authority int)");
        if (!createTable.exec())
        {
            qCritical() << createTable.lastError().text();
            return;
        }
    }

    if (!hasUser("Admin"))
    {
        _addUser("Admin", "SilicoolAdmin", Admin);
    }
    userModel = new MySqlTableModel(this, dbHelper.db);
    userModel->setTable("user");
    userModel->select();

    initUserNames();
    isInit = true;
}

int UserManagement::userCount() const
{
    QSqlQuery sqlStatement("select * from user", dbHelper.db);
    sqlStatement.exec();
    int countOfUser = 0;
    while (sqlStatement.next())
    {
        countOfUser++;
    }
    return countOfUser;
}

void UserManagement::addUser(QString userName, QString password, UserManagement::Authority authority)
{
    if (currentAuthority() != Admin)
    {
        UIOperation::getIns()->showMessage(tr("Authority Error"), tr("Only Admin can add user!"), MsgBoxIcon::Error, OkBtn);
        return;
    }
    int autho = static_cast<int>(authority);
    if (autho >= Admin || autho <= None)
    {
        UIOperation::getIns()->showMessage(tr("Authority Error"), tr("Please select correct authority!"), MsgBoxIcon::Error, OkBtn);
        return;
    }
    if (userName.length() < MinUserNameLen)
    {
        UIOperation::getIns()->showMessage(tr("UserName Error"), tr("User name was too short! Min length: %1").arg(MinUserNameLen), MsgBoxIcon::Error,
                                           OkBtn);
        return;
    }
    if (password.length() < MinPasswordLen)
    {
        UIOperation::getIns()->showMessage(tr("Password Error"), tr("Password was too short! Min length: %1").arg(MinPasswordLen), MsgBoxIcon::Error,
                                           OkBtn);
        return;
    }
    if (hasUser(userName))
    {
        UIOperation::getIns()->showMessage(tr("UserName Error"), tr("User name already existed!"), MsgBoxIcon::Error, OkBtn);
        return;
    }
    _addUser(userName, password, authority);
}

void UserManagement::removeUser(QString userName)
{
    if (currentAuthority() != Admin)
    {
        UIOperation::getIns()->showMessage(tr("Authority Error"), tr("Only Admin can remove user!"), MsgBoxIcon::Error, OkBtn);
        return;
    }

    QString password;
    Authority authority;
    if (!getUserInfo(userName, password, authority))
    {
        return;
    }
    if (authority == Admin)
    {
        UIOperation::getIns()->showMessage(tr("UserName Error"), tr("Can not remove Admin account!"), MsgBoxIcon::Error, OkBtn);
        return;
    }
    QSqlQuery remove(dbHelper.db);
    remove.prepare("delete from user where name=?");
    remove.addBindValue(userName);
    if (!remove.exec())
    {
        qCritical() << tr("Remove user %1 failed: ").arg(userName) << remove.lastError();
        return;
    }
    userModel->select();

    m_userNameList.removeOne(userName);
    emit userNameListChanged(m_userNameList);
}

bool UserManagement::changePassword(QString userName, QString oldPassword, QString newPassword, QString newPasswordRepeat)
{
    if (newPassword != newPasswordRepeat)
    {
        UIOperation::getIns()->showMessage(tr("Password Error"), tr("Repeat password error!"), MsgBoxIcon::Error, OkBtn);
        return false;
    }
    if (newPassword.length() < MinPasswordLen)
    {
        UIOperation::getIns()->showMessage(tr("Password Error"), tr("Password was too short! Min length: %1").arg(MinPasswordLen), MsgBoxIcon::Error,
                                           OkBtn);
        return false;
    }
    if (!verifyUserPsw(userName, oldPassword))
    {
        return false;
    }
    QSqlQuery changePsw(dbHelper.db);
    changePsw.prepare("update user set password=? where name=?");
    changePsw.addBindValue(cryptograph(newPassword));
    changePsw.addBindValue(userName);
    if (!changePsw.exec())
    {
        UIOperation::getIns()->showMessage(tr("Change password failed"), changePsw.lastError().text(), MsgBoxIcon::Error, OkBtn);
        return false;
    }
    return true;
}

bool UserManagement::login(QString userName, QString password)
{
    QString queriedPsw;
    Authority queriedAuthrity;
    if (getUserInfo(userName, queriedPsw, queriedAuthrity))
    {
        if (queriedPsw != cryptograph(password))
        {
            UIOperation::getIns()->showMessage(tr("Password Error"), tr("Please input correct password"), MsgBoxIcon::Error, OkBtn);
            return false;
        }
        setCurrentUserName(userName);
        setCurrentAuthority(queriedAuthrity);
        setHasLogin(true);
        return true;
    }
    return false;
}

void UserManagement::logout()
{
    setCurrentUserName("");
    setCurrentAuthority(None);
    setHasLogin(false);
}

void UserManagement::_addUser(QString userName, QString password, UserManagement::Authority authority)
{
    QSqlQuery sAddUser(dbHelper.db);
    sAddUser.prepare("insert into user(name, password, authority) values(?,?,?)");
    sAddUser.addBindValue(userName);
    sAddUser.addBindValue(cryptograph(password));
    sAddUser.addBindValue(static_cast<int>(authority));
    if (!sAddUser.exec())
    {
        qCritical() << tr("Add user %1 failed: ").arg(userName) << sAddUser.lastError();
        return;
    }
    userModel->select();

    m_userNameList.append(userName);
    emit userNameListChanged(m_userNameList);
}

bool UserManagement::hasUser(QString userName)
{
    QString queriedPsw;
    Authority queriedAuthrity;
    return getUserInfo(userName, queriedPsw, queriedAuthrity, false);
}

bool UserManagement::verifyUserPsw(QString userName, QString password)
{
    QString queriedPsw;
    Authority queriedAuthrity;
    if (getUserInfo(userName, queriedPsw, queriedAuthrity))
    {
        if (queriedPsw != cryptograph(password))
        {
            UIOperation::getIns()->showMessage(tr("Password Error"), tr("Please input correct password"), MsgBoxIcon::Error, OkBtn);
            return false;
        }
        return true;
    }
    return false;
}

bool UserManagement::getUserInfo(QString userName, QString &password, UserManagement::Authority &authority, bool showMsgBoxAsUserDidNotExist)
{
    QSqlQuery query(dbHelper.db);
    query.prepare("select * from user where name=?");
    query.addBindValue(userName);
    if (!query.exec())
    {
        UIOperation::getIns()->showMessage(tr("Query user %1 failed").arg(userName), query.lastError().text(), MsgBoxIcon::Error, OkBtn);
        return false;
    }
    if (query.next())
    {
        password = query.value("password").toString();
        authority = static_cast<Authority>(query.value("authority").toInt());
        return true;
    }
    else
    {
        if (showMsgBoxAsUserDidNotExist)
        {
            UIOperation::getIns()->showMessage(tr("UserName Error"), tr("User %1 did not exist").arg(userName), MsgBoxIcon::Error, OkBtn);
        }
        return false;
    }
}

void UserManagement::initUserNames()
{
    m_userNameList.clear();
    QSqlQuery sqlStatement("select * from user", dbHelper.db);
    sqlStatement.exec();
    while (sqlStatement.next())
    {
        m_userNameList.append(sqlStatement.value("name").toString());
    }
    emit userNameListChanged(m_userNameList);
}
