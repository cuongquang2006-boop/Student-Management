#ifndef COURSESMANAGERWIDGET_H
#define COURSESMANAGERWIDGET_H

#include <QWidget>
#include <QVector>
#include <QRegularExpression>
#include "Course.h"
#include "Student.h"

class QTableWidget;
class QLineEdit;
class QPushButton;
class QComboBox;

class CoursesManagerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CoursesManagerWidget(QWidget* parent = nullptr);

    void setStudentList(const QVector<Student>& students);
    void setCourses(const QVector<Course>& list);

    const QVector<Course>& getCourses() const;
    const QVector<Student>& getStudentList() const;

signals:
    void backRequested();

private:
    QVector<Course> courses;
    QVector<Student> studentList;

    QTableWidget* table;

    QLineEdit* idEdit;
    QLineEdit* nameEdit;
    QLineEdit* teacherEdit;

    QComboBox* studentBox;
    QComboBox* classBox;

    void setupUI();
    void setupConnections();

    void refreshTable();
    void updateStudentCombo(int row);
    bool isValidCourseId(const QString& id);
    bool isValidTeacherName(const QString& name);

private slots:
    void addCourse();
    void deleteCourse();
    void addStudentToCourse();
    void onViewStudents(); 
};

#endif
