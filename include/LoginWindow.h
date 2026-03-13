#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QVector>
#include "User.h"

class LoginWindow : public QWidget
{
    Q_OBJECT

public:
    LoginWindow();

private:
    QLineEdit* usernameEdit;
    QLineEdit* passwordEdit;

    QPushButton* loginBtn;
    QPushButton* registerBtn;

    QVector<User> users;

    void setupUI();
    void loadUsers();
    void saveUsers();

private slots:
    void login();
    void registerUser();
};

#endif
