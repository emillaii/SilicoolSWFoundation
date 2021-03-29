#ifndef USERMANAGEMENT_H
#define USERMANAGEMENT_H

#include "mysqltablemodel.h"
#include "uiHelper/uioperation.h"
#include "utilities_global.h"
#include <QCryptographicHash>
#include <QDebug>
#include <QObject>
#include <qsqldatabase.h>
#include <qsqlerror.h>
#include <qsqlquery.h>

class UTILITIESSHARED_EXPORT UserManagement : public QObject
{
    Q_OBJECT

public:
    UserManagement();
    ~UserManagement();

    enum Authority
    {
        None,
        Operator,
        Engineer,
        Admin,
    };
    Q_ENUM(Authority)

    static EnumHelper<Authority> &AuthorityEnumInfo()
    {
        static EnumHelper<Authority> instance(staticMetaObject, "Authority");
        return instance;
    }

    Q_PROPERTY(QString currentUserName READ currentUserName WRITE setCurrentUserName NOTIFY currentUserNameChanged)
    Q_PROPERTY(Authority currentAuthority READ currentAuthority WRITE setCurrentAuthority NOTIFY currentAuthorityChanged)
    Q_PROPERTY(QString currentAuthorityName READ currentAuthorityName WRITE setCurrentAuthorityName NOTIFY currentAuthorityNameChanged)
    Q_PROPERTY(bool hasLogin READ hasLogin WRITE setHasLogin NOTIFY hasLoginChanged)
    Q_PROPERTY(QStringList userNameList READ userNameList NOTIFY userNameListChanged)

    void init();

    Q_INVOKABLE int userCount() const;
    Q_INVOKABLE void addUser(QString userName, QString password, Authority authority);
    Q_INVOKABLE void removeUser(QString userName);
    Q_INVOKABLE bool changePassword(QString userName, QString oldPassword, QString newPassword, QString newPasswordRepeat);

    Q_INVOKABLE bool login(QString userName, QString password);
    Q_INVOKABLE void logout();

    QString currentUserName() const
    {
        return m_currentUserName;
    }
    Authority currentAuthority() const
    {
        return m_currentAuthority;
    }
    bool hasLogin() const
    {
        return m_hasLogin;
    }
    QString currentAuthorityName() const
    {
        return m_currentAuthorityName;
    }
    QStringList userNameList() const
    {
        return m_userNameList;
    }

public slots:
    void setCurrentUserName(QString currentUserName)
    {
        if (m_currentUserName == currentUserName)
            return;

        m_currentUserName = currentUserName;
        emit currentUserNameChanged(m_currentUserName);
    }
    void setCurrentAuthority(Authority currentAuthority)
    {
        if (m_currentAuthority == currentAuthority)
            return;

        m_currentAuthority = currentAuthority;
        emit currentAuthorityChanged(m_currentAuthority);
        setCurrentAuthorityName(AuthorityEnumInfo().enumToName(currentAuthority));
    }
    void setHasLogin(bool hasLogin)
    {
        if (m_hasLogin == hasLogin)
            return;

        m_hasLogin = hasLogin;
        emit hasLoginChanged(m_hasLogin);
    }
    void setCurrentAuthorityName(QString currentAuthorityName)
    {
        if (m_currentAuthorityName == currentAuthorityName)
            return;

        m_currentAuthorityName = currentAuthorityName;
        emit currentAuthorityNameChanged(m_currentAuthorityName);
    }

signals:
    void currentUserNameChanged(QString currentUserName);
    void currentAuthorityChanged(Authority currentAuthority);
    void hasLoginChanged(bool hasLogin);
    void currentAuthorityNameChanged(QString currentAuthorityName);
    void userNameListChanged(QStringList userNameList);

private:
    QString cryptograph(QString clearText)
    {
        return QCryptographicHash::hash(clearText.toUtf8(), QCryptographicHash::Md5).toHex();
    }
    bool openDB()
    {
        if (!db.open())
        {
            qCritical() << tr("Open db failed: ") << db.lastError().text();
            return false;
        }
        return true;
    }
    void closeDB()
    {
        db.close();
    }
    void _addUser(QString userName, QString password, Authority authority);
    bool hasUser(QString userName);
    bool isTableExist(QString tableName);
    bool verifyUserPsw(QString userName, QString password);
    bool getUserInfo(QString userName, QString &password, Authority &authority, bool showMsgBoxAsUserDidNotExist = true);
    void initUserNames();

public:
    MySqlTableModel *userModel;

private:
    QString m_currentUserName = "";
    Authority m_currentAuthority = None;
    QString m_currentAuthorityName = "None";
    QSqlDatabase db;
    const QString ConnectionName = "qt_sql_userManagement_connection";
    const QString DBName = "Users.db";
    const QString DBUserName = "Silicol";
    const QString DBPassword = "Silicol_Psw";
    const int MinUserNameLen = 4;
    const int MinPasswordLen = 6;
    bool isInit = false;
    bool m_hasLogin = false;
    QStringList m_userNameList;
};

#endif    // USERMANAGEMENT_H
