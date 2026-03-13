#ifndef USER_H
#define USER_H

#include <QString>
#include <QJsonObject>

struct User
{
    QString username;
    QString password;

    QJsonObject toJson() const
    {
        QJsonObject obj;
        obj["username"] = username;
        obj["password"] = password;
        return obj;
    }

    static User fromJson(const QJsonObject &obj)
    {
        User u;
        u.username = obj["username"].toString();
        u.password = obj["password"].toString();
        return u;
    }
};

#endif
