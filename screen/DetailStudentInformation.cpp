#include "DetailStudentInformation.h"

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QPixmap>

#include <QFileDialog>

#include <QGraphicsDropShadowEffect>

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
    photoLabel->setFixedSize(170,220);
    photoLabel->setAlignment(Qt::AlignCenter);

    photoLabel->setStyleSheet(
        "border:2px solid #2aa198;"
        "border-radius:12px;"
        "background:#1e1e1e;"
        );

    QPixmap avatar(s.avatarPath.isEmpty() ? ":/images/default_avatar.png" : s.avatarPath);

    photoLabel->setPixmap(
                      avatar.scaled(photoLabel->size(),
                      Qt::KeepAspectRatioByExpanding,
                      Qt::SmoothTransformation));


    idLabel = new QLabel(s.id);
    nameLabel = new QLabel(s.name);
    ageLabel = new QLabel(QString::number(s.age));
    genderLabel = new QLabel(s.gender);
    majorLabel = new QLabel(s.major);
    classLabel = new QLabel(s.className);
    schoolLabel = new QLabel("University Of The Transport In Ho Chi Minh City (UTH)");


    QFormLayout *infoLayout = new QFormLayout;
    infoLayout->setSpacing(10);
    infoLayout->setLabelAlignment(Qt::AlignRight);

    infoLayout->addRow("ID:", idLabel);
    infoLayout->addRow("Name:", nameLabel);
    infoLayout->addRow("Age:", ageLabel);
    infoLayout->addRow("Gender:", genderLabel);
    infoLayout->addRow("Major:", majorLabel);
    infoLayout->addRow("Class:", classLabel);
    infoLayout->addRow("School:", schoolLabel);


    QWidget *infoWidget = new QWidget;
    infoWidget->setObjectName("infoWidget");
    infoWidget->setLayout(infoLayout);

    infoWidget->setStyleSheet(
        "#infoWidget{"
        "background:#2b2b2b;"
        "border:1px solid #3c3c3c;"
        "border-radius:12px;"
        "padding:15px;"
        "}"
        "QLabel{"
        "color:#e6e6e6;"
        "font-size:14px;"
        "}"
        );


    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect;
    shadow->setBlurRadius(20);
    shadow->setOffset(0,4);
    shadow->setColor(QColor(0,0,0,120));
    infoWidget->setGraphicsEffect(shadow);


    chooseavtBtn = new QPushButton("Choose Avatar");
    closeBtn = new QPushButton("Close");

    chooseavtBtn->setStyleSheet(
        "QPushButton{"
        "background:#4CAF50;"
        "color:white;"
        "padding:6px 15px;"
        "border-radius:6px;"
        "}"
        "QPushButton:hover{"
        "background:#45a049;"
        "}"
        );

    closeBtn->setStyleSheet(
        "QPushButton{"
        "background:#e74c3c;"
        "color:white;"
        "padding:6px 15px;"
        "border-radius:6px;"
        "}"
        );


    connect(chooseavtBtn,&QPushButton::clicked, 
        this,&DetailStudentInformation::chooseAvatar);

    connect(closeBtn,&QPushButton::clicked,
        this,&QDialog::accept);


    QVBoxLayout *mainLayout = new QVBoxLayout;

    mainLayout->addWidget(photoLabel,0,Qt::AlignCenter);
    mainLayout->addWidget(chooseavtBtn,0,Qt::AlignCenter);
    mainLayout->addWidget(infoWidget);
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
    photoLabel->setPixmap(
        avatar.scaled(photoLabel->size(),
                      Qt::KeepAspectRatioByExpanding,
                      Qt::SmoothTransformation)
        );
}

Student DetailStudentInformation::getStudent() const
{
    return s;
}
