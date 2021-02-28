#ifndef MSGBOXMODEL_H
#define MSGBOXMODEL_H

#include "../utilities_global.h"
#include "commonmethod.h"
#include "enumhelper.h"
#include <QAbstractListModel>
#include <QObject>

class UTILITIESSHARED_EXPORT MsgBoxIcon : public QObject
{
    Q_OBJECT

public:
    enum Icon
    {
        Question,
        Information,
        Warning,
        Error
    };
    Q_ENUM(Icon)

    static EnumHelper<Icon> &IconEnumInfo()
    {
        static EnumHelper<Icon> ins(MsgBoxIcon::staticMetaObject, "Icon");
        return ins;
    }
};

Q_DECLARE_METATYPE(MsgBoxIcon::Icon)

enum MsgBoxRole
{
    UuidRole = Qt::UserRole + 1,
    TitleRole,
    IconRole,
    ContentRole,
    ButtonsRole
};

class MsgBoxItem
{
public:
    MsgBoxItem() = default;

    MsgBoxItem(const QString &uuid,
               const QString &title,
               MsgBoxIcon::Icon icon,
               const QString &content,
               const QList<QString> &buttons)
    {
        this->uuid = uuid;
        this->title = title;
        this->icon = icon;
        this->content = content;
        this->buttons = buttons;
    }

public:
    QString uuid;
    QString title;
    MsgBoxIcon::Icon icon;
    QString content;
    QList<QString> buttons;
};

Q_DECLARE_METATYPE(MsgBoxItem)

class UTILITIESSHARED_EXPORT MsgBoxModel : public QAbstractListModel
{
    Q_OBJECT

public:
    MsgBoxModel();

    // QAbstractItemModel interface
public:
    virtual int rowCount(const QModelIndex &parent) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual QHash<int, QByteArray> roleNames() const override;

    void addMsgBox(const MsgBoxItem &msgBox);
    void removeMsgBox(const QString &uuid);

signals:
    void msgBoxCountChanged(int count);

private:
    QList<MsgBoxItem> msgBoxes;
    QHash<int, QByteArray> msgBoxRoleNames;
};

#endif    // MSGBOXMODEL_H
