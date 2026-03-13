#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>  
#include <QTableWidget> 
#include <QLineEdit>     
#include <QSpinBox>     
#include <QComboBox>    
#include <QPushButton>  
#include <QVector>     
#include "Student.h"   

class MainWindow : public QMainWindow
{
    Q_OBJECT   

public:
    MainWindow(QWidget* parent = nullptr);

private:
    QVector<Student> students;
    QTableWidget* table;
    
    QLineEdit* idEdit;
    QLineEdit* nameEdit;
    QSpinBox* ageSpin;
    QComboBox* genderBox;
    QLineEdit* majorEdit;
    QComboBox* classBox;
    QLineEdit* searchEdit;

    QPushButton* addBtn;
    QPushButton* updateBtn;
    QPushButton* deleteBtn;
    QPushButton* clearBtn;

    void setupUI();
    void setupMenu();
    void setupToolbar();
    void refreshTable();
    void clearForm();

private slots:
    void addStudent();
    void updateStudent();
    void deleteStudent();
    void loadToForm(int row, int col);
    void searchStudent(const QString& text);
    void saveToFile();
    void loadFromFile();

};

#endif
