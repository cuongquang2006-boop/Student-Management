#ifndef COURSE_H
#define COURSE_H

#include <QString>
#include <QStringList>
#include <QJsonObject>
#include <QJsonArray>

class Course
{
public:
    QString id;
    QString name;
    QString teacher;
    QString className;
    QStringList studentIds; 

    Course() {}

    Course(QString id, QString name, QString teacher, QString className)
        : id(id), name(name), teacher(teacher), className(className) {
    }

    QJsonObject toJson() const
    {
        QJsonObject obj;
        obj["id"] = id;
        obj["name"] = name;
        obj["teacher"] = teacher;
        obj["class"] = className;

        QJsonArray arrStudents;
        for (const QString& s : studentIds) arrStudents.append(s);
        obj["students"] = arrStudents;

        return obj;
    }

    static Course fromJson(const QJsonObject& obj)
    {
        Course c(
            obj["id"].toString(),
            obj["name"].toString(),
            obj["teacher"].toString(),
            obj["class"].toString()
        );

        QJsonArray arrStudents = obj["students"].toArray();
        for (const QJsonValue& v : arrStudents)
            c.studentIds.append(v.toString());

        return c;
    }

    void addStudent(const QString& studentId) {
        if (!studentIds.contains(studentId))
            studentIds.append(studentId);
    }

    void removeStudent(const QString& studentId) {
        studentIds.removeAll(studentId);
    }
};

#endif
