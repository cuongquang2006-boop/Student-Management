
#ifndef STUDENT_H
#define STUDENT_H

#include <QString>       
#include <QJsonObject>    

class Student
{
public:

    QString id;      
    QString name;    
    int age;
    QString gender;
    QString major;
    QString className;
    QString avatarPath;  ///student face image

    Student() {}

    Student(QString id, QString name, int age, QString gender, QString major,QString className,QString avatarPath = "")
        : id(id), name(name), age(age), gender(gender), major(major),className(className), avatarPath(avatarPath) {}

    QJsonObject toJson() const
    {
        QJsonObject obj;     

        obj["id"] = id;
        obj["name"] = name;
        obj["age"] = age;
        obj["gender"] = gender;
        obj["major"] = major;
        obj["class"] = className;
        obj["image"] = avatarPath;

        return obj; 
    }

    static Student fromJson(const QJsonObject &obj)
    {
        return Student(
            obj["id"].toString(),
            obj["name"].toString(),
            obj["age"].toInt(),
            obj["gender"].toString(),
            obj["major"].toString(),
            obj["class"].toString(),
            obj["image"].toString()
            );
    }
};

#endif
