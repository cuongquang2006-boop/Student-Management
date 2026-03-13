#include "DetailStudentInformation.h"

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QPixmap>

#include <QFileDialog>

DetailStudentInformation::DetailStudentInformation(const Student &student, QWidget *parent)
    : QDialog(parent), s(student)
{
    inforUI();
    setWindowTitle("Student Detail");
    resize(900,600);
}

void DetailStudentInformation::inforUI()
{
        photoLabel = new QLabel;
        photoLabel->setFixedSize(150,150);
        photoLabel->setStyleSheet("border:1px solid gray;");
        photoLabel->setAlignment(Qt::AlignCenter);
        photoLabel->setScaledContents(true);       //ti le anh can doi

        QPixmap avatar;

        if(s.avatarPath.isEmpty())
        {
            avatar.load(":/images/default_avatar.png");
        }
        else
        {
            avatar.load(s.avatarPath);
        }

        photoLabel->setPixmap(avatar.scaled(150,150,Qt::KeepAspectRatio));


        idLabel = new QLabel(s.id);
        nameLabel = new QLabel(s.name);
        ageLabel = new QLabel(QString::number(s.age));
        genderLabel = new QLabel(s.gender);
        majorLabel = new QLabel(s.major);
        classLabel = new QLabel(s.className);
        schoolLabel = new QLabel("University Of The Transport In Ho Chi Minh City (UTH)");

        QFormLayout *infoLayout = new QFormLayout;

        infoLayout->addRow("ID:", idLabel);
        infoLayout->addRow("Name:", nameLabel);
        infoLayout->addRow("Age:", ageLabel);
        infoLayout->addRow("Gender:", genderLabel);
        infoLayout->addRow("Major:", majorLabel);
        infoLayout->addRow("Class:", classLabel);
        infoLayout->addRow("School:",schoolLabel);

        chooseavtBtn = new QPushButton("Choose Avatar");
        closeBtn = new QPushButton("Close");

        connect(chooseavtBtn,&QPushButton::clicked, this,&DetailStudentInformation::chooseAvatar);
        connect(closeBtn, &QPushButton::clicked,this, &QDialog::accept);

        QVBoxLayout *mainLayout = new QVBoxLayout;

        mainLayout->addWidget(photoLabel,0,Qt::AlignCenter);
        mainLayout->addWidget(chooseavtBtn,0,Qt::AlignCenter);
        mainLayout->addLayout(infoLayout);
        mainLayout->addWidget(closeBtn,0,Qt::AlignCenter);

        setLayout(mainLayout);
}

void DetailStudentInformation::chooseAvatar()
{
    QString file = QFileDialog::getOpenFileName(
        this,"Choose Avatar","","Images (*.png *.jpg *.jpeg)");

    if(file.isEmpty())
        return;

    s.avatarPath = file;

    QPixmap avatar(file);
    photoLabel->setPixmap(avatar.scaled(150,150,Qt::KeepAspectRatio));
}

Student DetailStudentInformation::getStudent() const
{
    return s;
}
