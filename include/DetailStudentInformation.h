#ifndef DETAILSTUDENTINFORMATION_H
#define DETAILSTUDENTINFORMATION_H

#include <QDialog>
#include "Student.h"

class QLabel;
class QPushButton;

class DetailStudentInformation : public QDialog
{
    Q_OBJECT
public:
    explicit DetailStudentInformation(const Student &student, QWidget *parent = nullptr);

    Student getStudent() const;
private:
    Student s;

    QLabel * photoLabel;

    QLabel* idLabel;
    QLabel* nameLabel;
    QLabel* ageLabel;
    QLabel* genderLabel;
    QLabel* majorLabel;
    QLabel* classLabel;
    QLabel* schoolLabel;

    QPushButton* closeBtn;
    QPushButton* chooseavtBtn;

    void inforUI();
    void chooseAvatar();

};

#endif // DETAILSTUDENTINFORMATION_H
