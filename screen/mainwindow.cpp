#include "MainWindow.h"

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QFrame>
#include <QLabel>
#include <QStatusBar>
#include <QMessageBox>

#include <QPropertyAnimation>   
#include <QEasingCurve>         

#include <QFileDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>

void MainWindow::setActiveButton(QPushButton* btn)
{
    for (QPushButton* b : menuButtons)
    {
        b->setStyleSheet(R"(
            QPushButton{
                text-align:left;
                padding:10px;
                border:none;
                color:white;
                font-size:15px;
                border-radius:6px;
                background:transparent;
            }

            QPushButton:hover{
                background:#2d7df6;
            }
        )");
    }

    btn->setStyleSheet(R"(
        QPushButton{
            text-align:left;
            padding:10px;
            border:none;
            color:white;
            font-size:15px;
            border-radius:6px;
            background:#2d7df6;
        }
    )");
}

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    QWidget* central = new QWidget;
    QHBoxLayout* mainLayout = new QHBoxLayout(central);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    QFrame* sideBar = new QFrame;
    sideBar->setFixedWidth(200);

    QVBoxLayout* sideLayout = new QVBoxLayout(sideBar);
    sideLayout->setSpacing(10);
    sideLayout->setContentsMargins(10, 20, 10, 10);

    QPushButton* btnDashboard = new QPushButton("Dashboard");
    QPushButton* btnStudent = new QPushButton("Student Manager");
    QPushButton* btnCourses = new QPushButton("Courses");
    QPushButton* btnScores = new QPushButton("Scores");

    menuButtons = {
        btnDashboard,
        btnStudent,
        btnCourses,
        btnScores,
    };

    for (QPushButton* btn : menuButtons)
        sideLayout->addWidget(btn);

    sideLayout->addStretch();

    QPushButton* btnSave = new QPushButton("Save All");
    QPushButton* btnLoad = new QPushButton("Load All");

    sideLayout->addWidget(btnSave);
    sideLayout->addWidget(btnLoad);

    stack = new QStackedWidget;

    dashboardWidget = new DashboardWidget;
    stack->addWidget(dashboardWidget);

    studentWidget = new StudentManagerWidget;
    connect(studentWidget, &StudentManagerWidget::backRequested,
        this, &MainWindow::openDashboard);
    stack->addWidget(studentWidget);

    courseWidget = new CoursesManagerWidget;
    stack->addWidget(courseWidget);

    scoreWidget = new ScoreManagerWidget;
    stack->addWidget(scoreWidget);

    courseWidget->setStudentList(studentWidget->getStudents());

    scoreWidget->setData(
        studentWidget->getStudents(),
        courseWidget->getCourses(),
        {}
    );

    connect(studentWidget, &StudentManagerWidget::dataChanged,
        this, [this]()
        {
            auto students = studentWidget->getStudents();
            auto courses = courseWidget->getCourses();

            courseWidget->setStudentList(students);

            scoreWidget->setData(
                students,
                courses,
                scoreWidget->getScores()
            );
        });

    mainLayout->addWidget(sideBar);
    mainLayout->addWidget(stack);

    setCentralWidget(central);

    connect(btnDashboard, &QPushButton::clicked, this, [=]() {
            dashboardWidget->setData(
            studentWidget->getStudents(),
            courseWidget->getCourses(),
            scoreWidget->getScores()
        );
        openDashboard();
        setActiveButton(btnDashboard);
        });

    connect(btnStudent, &QPushButton::clicked, this, [=]() {
        openStudent();
        setActiveButton(btnStudent);
        });

    connect(btnCourses, &QPushButton::clicked, this, [=]() {
        openCourses();
        setActiveButton(btnCourses);
        });

    connect(btnScores, &QPushButton::clicked, this, [=]() {

        scoreWidget->setData(
            studentWidget->getStudents(),
            courseWidget->getCourses(),
            scoreWidget->getScores()
        );

        openScores();
        setActiveButton(btnScores);
        });

    connect(btnSave, &QPushButton::clicked, this, &MainWindow::saveAll);
    connect(btnLoad, &QPushButton::clicked, this, &MainWindow::loadAll);

    sideBar->setStyleSheet(R"(
        QFrame{ background:#1f1f1f; }
    )");

    statusBar()->showMessage("Ready");
    resize(1000, 650);

    setActiveButton(btnDashboard);
}

void MainWindow::saveAll()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save All", "", "*.json");
    if (fileName.isEmpty()) return;

    QJsonObject root;

    QJsonArray studentArr;
    for (const Student& s : studentWidget->getStudents())
        studentArr.append(s.toJson());
    root["students"] = studentArr;

    QJsonArray courseArr;
    for (const Course& c : courseWidget->getCourses())
        courseArr.append(c.toJson());
    root["courses"] = courseArr;

    QJsonArray scoreArr;
    for (const Score& sc : scoreWidget->getScores())
        scoreArr.append(sc.toJson());
    root["scores"] = scoreArr;

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::warning(this, "Error", "Cannot save file!");
        return;
    }

    file.write(QJsonDocument(root).toJson());
    file.close();

    studentWidget->setModified(false);

    QMessageBox::information(this, "OK", "Saved ALL data!");
}

void MainWindow::loadAll()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Load All", "", "*.json");
    if (fileName.isEmpty()) return;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(this, "Error", "Cannot open file!");
        return;
    }

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    QJsonObject root = doc.object();

    QVector<Student> students;
    for (auto v : root["students"].toArray())
        students.append(Student::fromJson(v.toObject()));

    studentWidget->setStudents(students);

    QVector<Course> courses;
    for (auto v : root["courses"].toArray())
        courses.append(Course::fromJson(v.toObject()));

    courseWidget->setCourses(courses);
    courseWidget->setStudentList(students);

    QVector<Score> scores;
    for (auto v : root["scores"].toArray())
        scores.append(Score::fromJson(v.toObject()));

    scoreWidget->setData(
        students,
        courses,
        scores
    );

    studentWidget->setModified(false);

    QMessageBox::information(this, "OK", "Loaded ALL data!");
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    if (studentWidget->isModified())
    {
        auto reply = QMessageBox::question(
            this,
            "Confirm Exit",
            "You have unsaved changes. Exit anyway?",
            QMessageBox::Yes | QMessageBox::No
        );

        if (reply == QMessageBox::No)
        {
            event->ignore();
            return;
        }
    }

    event->accept();
}

void MainWindow::openDashboard()
{
    stack->setCurrentWidget(dashboardWidget);
    dashboardWidget->move(0, -stack->height());

    QPropertyAnimation* anim = new QPropertyAnimation(dashboardWidget, "pos");
    anim->setDuration(250);
    anim->setStartValue(dashboardWidget->pos());
    anim->setEndValue(QPoint(0, 0));
    anim->setEasingCurve(QEasingCurve::OutCubic);
    anim->start(QAbstractAnimation::DeleteWhenStopped);
}

void MainWindow::openStudent()
{
    stack->setCurrentWidget(studentWidget);
    studentWidget->move(stack->width(), 0);

    QPropertyAnimation* anim = new QPropertyAnimation(studentWidget, "pos");
    anim->setDuration(220);
    anim->setStartValue(studentWidget->pos());
    anim->setEndValue(QPoint(0, 0));
    anim->setEasingCurve(QEasingCurve::OutCubic);
    anim->start(QAbstractAnimation::DeleteWhenStopped);
}

void MainWindow::openCourses()
{
    stack->setCurrentWidget(courseWidget);
    courseWidget->move(stack->width(), 0);

    QPropertyAnimation* anim = new QPropertyAnimation(courseWidget, "pos");
    anim->setDuration(220);
    anim->setStartValue(courseWidget->pos());
    anim->setEndValue(QPoint(0, 0));
    anim->setEasingCurve(QEasingCurve::OutCubic);
    anim->start(QAbstractAnimation::DeleteWhenStopped);
}

void MainWindow::openScores()
{
    stack->setCurrentWidget(scoreWidget);
    scoreWidget->move(stack->width(), 0);

    QPropertyAnimation* anim = new QPropertyAnimation(scoreWidget, "pos");
    anim->setDuration(220);
    anim->setStartValue(scoreWidget->pos());
    anim->setEndValue(QPoint(0, 0));
    anim->setEasingCurve(QEasingCurve::OutCubic);
    anim->start(QAbstractAnimation::DeleteWhenStopped);
}
