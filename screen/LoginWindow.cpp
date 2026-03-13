#include "LoginWindow.h"
#include "MainWindow.h"

#include <QVBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>
#include <QMessageBox>

LoginWindow::LoginWindow()
{
    setupUI();
    loadUsers();
}

void LoginWindow::setupUI()
{
    QFormLayout *formLayout = new QFormLayout;
    formLayout->setSpacing(12);

    usernameEdit = new QLineEdit;
    passwordEdit = new QLineEdit;
    passwordEdit->setEchoMode(QLineEdit::Password);

    usernameEdit->setMinimumHeight(32);
    passwordEdit->setMinimumHeight(32);

    loginBtn = new QPushButton("Login");
    registerBtn = new QPushButton("Register");

    loginBtn->setMinimumHeight(35);
    registerBtn->setMinimumHeight(35);

    formLayout->addRow("Username:", usernameEdit);
    formLayout->addRow("Password:", passwordEdit);

    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->setSpacing(10);
    btnLayout->addWidget(loginBtn);
    btnLayout->addWidget(registerBtn);

    formLayout->addRow(btnLayout);

    // Widget chứa form (card)
    QWidget *formWidget = new QWidget;
    formWidget->setLayout(formLayout);
    formWidget->setMaximumWidth(400);
    formWidget->setObjectName("formCard");

    // Layout chính để căn giữa
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addStretch();
    mainLayout->addWidget(formWidget, 0, Qt::AlignCenter);
    mainLayout->addStretch();

    setLayout(mainLayout);
    setWindowTitle("Login");
    resize(700, 500);

    // Style cho card
    setStyleSheet(R"(
        #formCard{
            background-color: rgba(255,255,255,0.06);
            border-radius: 10px;
            padding: 25px;
            border: 1px solid rgba(255,255,255,0.15);
        }
    )");

    connect(loginBtn, &QPushButton::clicked, this, &LoginWindow::login);
    connect(registerBtn, &QPushButton::clicked, this, &LoginWindow::registerUser);
}

void LoginWindow::login()
{
    QString user = usernameEdit->text();
    QString pass = passwordEdit->text();

    for(const User& u : users)
    {
        if(u.username == user && u.password == pass)
        {
            MainWindow* w = new MainWindow();
            w->show();
            close();
            return;
        }
    }

    QMessageBox::warning(this,"Fail","Wrong username or password");
}

void LoginWindow::registerUser()
{
    QString user = usernameEdit->text();
    QString pass = passwordEdit->text();

    //không để trống mục nhập mật khẩu hoặc user
    if(user.isEmpty() || pass.isEmpty())
    {
        QMessageBox::warning(this,"Error","Fill all fields");
        return;
    }

    for(const User& u : users)
    {
        //không để trùng tên người dùng
        if(u.username == user)
        {
            QMessageBox::warning(this,"Error","Username exists");
            return;
        }
    }

    users.append({user,pass});
    saveUsers();

    QMessageBox::information(this,"Success","Account created");
}

void LoginWindow::loadUsers()
{
    QFile file("users.json");
    if(!file.open(QIODevice::ReadOnly)) return;

    QJsonArray arr = QJsonDocument::fromJson(file.readAll()).array();

    users.clear();

    for(auto v : arr)
        users.append(User::fromJson(v.toObject()));
}

void LoginWindow::saveUsers()
{
    QJsonArray arr;

    for(const User& u : users)
        arr.append(u.toJson());

    QFile file("users.json");
    file.open(QIODevice::WriteOnly);

    file.write(QJsonDocument(arr).toJson());
}
