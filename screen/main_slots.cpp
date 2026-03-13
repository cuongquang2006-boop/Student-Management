#include "MainWindow.h"

#include <QFileDialog>
#include <QStatusBar>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>
#include <QLabel>
#include <QMessageBox>


void MainWindow::addStudent()
{
    //khong de trong name va id
    if(idEdit->text().isEmpty() || nameEdit->text().isEmpty())
    {
        QMessageBox::warning(this,"Fail!","ID and name required!");
        statusBar()->showMessage("ID and name required!");
        return;
    }

    //khong de trung id
    for(const Student& s : students)
    {
        if(s.id == idEdit->text())
        {
            QMessageBox::warning(this,"Duplicate"," ID already exists!");
            return;
        }
    }

    students.append(Student(
        idEdit->text(),
        nameEdit->text(),
        ageSpin->value(),
        genderBox->currentText(),
        majorEdit->text(),
        classBox->currentText(),
        ""
        ));

    refreshTable();
    statusBar()->showMessage("Added student");
}

void MainWindow::updateStudent()
{
    int row = table->currentRow();

    //phai chon student
    if (row < 0)
    {
        QMessageBox::warning(this,"Error","Please select a student first!");
        return;
    }

    QString oldAvatar = students[row].avatarPath;

    students[row] = Student(
        idEdit->text(),
        nameEdit->text(),
        ageSpin->value(),
        genderBox->currentText(),
        majorEdit->text(),
        classBox->currentText(),
        oldAvatar
        );

    refreshTable();
}

void MainWindow::deleteStudent()
{
    int row = table->currentRow();
    if (row < 0) return;

    if(QMessageBox::question(this,"Comfirm Delete","Delete this student?")!=QMessageBox::Yes) return;

    students.removeAt(row);
    refreshTable();
}

void MainWindow::searchStudent(const QString &text)
{
    for (int i = 0; i < table->rowCount(); ++i)
    {
        bool match = table->item(i, 1)->text().contains(text, Qt::CaseInsensitive);
        table->setRowHidden(i, !match);
    }
}

void MainWindow::saveToFile()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save", "", "*.json");
    if (fileName.isEmpty()) return;

    QJsonArray arr;

    for (const Student &s : students)
        arr.append(s.toJson());

    QFile file(fileName);
    file.open(QIODevice::WriteOnly);

    file.write(QJsonDocument(arr).toJson());
}

void MainWindow::loadFromFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open", "", "*.json");
    if (fileName.isEmpty()) return;

    QFile file(fileName);
    file.open(QIODevice::ReadOnly);

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    QJsonArray arr = doc.array();

    students.clear();

    for (auto v : arr)
        students.append(Student::fromJson(v.toObject()));

    refreshTable();
}
