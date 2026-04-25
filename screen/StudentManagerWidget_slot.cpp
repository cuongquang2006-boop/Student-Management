#include "StudentManagerWidget.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QTableWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>

#include <QRegularExpression>
#include <QFile>

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include <QTextCursor>

void StudentManagerWidget::addStudent()
{
    QString id = idEdit->text().trimmed();
    QString name = nameEdit->text().trimmed();
    QString major = majorBox->currentText().trimmed();
    QString className = classBox->currentText();

    QRegularExpression idRegex("^\\d+$");
    QRegularExpression nameRegex("^[\\p{L} ]+$");
    QRegularExpression textRegex("^[\\p{L}0-9 ]+$");

    if (id.isEmpty() || name.isEmpty())
    {
        QMessageBox::warning(this, "Add Fail!", "ID and name required!");
        return;
    }

    if (!idRegex.match(id).hasMatch())
    {
        QMessageBox::warning(this, "Add Fail!", "Student ID must contain digits only.");
        return;
    }

    if (id.length() > 9)
    {
        QMessageBox::warning(this, "Add Fail!", "Student ID must not exceed 9 digits.");
        return;
    }

    if (!id.startsWith("08"))
    {
        QMessageBox::warning(this, "Add Fail!", "Student ID must start with 08.");
        return;
    }

    if (!nameRegex.match(name).hasMatch())
    {
        QMessageBox::warning(this, "Add Fail!", "Name must contain letters only.");
        return;
    }

    if (major == "<<None>>")
    {
        QMessageBox::warning(this, "Add Fail!", "Please select a major.");
        return;
    }

    if (!textRegex.match(major).hasMatch())
    {
        QMessageBox::warning(this, "Add Fail!", "Major contains invalid characters.");
        return;
    }

    if (genderBox->currentText() == "<<None>>")
    {
        QMessageBox::warning(this, "Add Fail!", "Please select a gender.");
        return;
    }

    if (className == "<<None>>")
    {
        QMessageBox::warning(this, "Add Fail!", "Please select a class.");
        return;
    }

    for (const Student& s : students)
    {
        if (s.id == id)
        {
            QMessageBox::warning(this, "Duplicate", "ID already exists!");
            return;
        }
    }

    students.append(Student(
        id,
        name,
        ageSpin->value(),
        genderBox->currentText(),
        major,
        className,
        ""
    ));

    modified = true;

    emit dataChanged();

    refreshTable();

}


void StudentManagerWidget::updateStudent()
{
    int row = table->currentRow();

    if (row < 0)
    {
        QMessageBox::warning(this, "Error", "Please select a student first!");
        return;
    }

    QString id = idEdit->text().trimmed();
    QString name = nameEdit->text().trimmed();
    QString major = majorBox->currentText().trimmed();
    QString className = classBox->currentText();
    QString gender = genderBox->currentText();
    int age = ageSpin->value();

    QRegularExpression idRegex("^\\d+$");          
    QRegularExpression nameRegex("^[\\p{L} ]+$");   
    QRegularExpression textRegex("^[\\p{L}0-9 ]+$");

    if (id.isEmpty() || name.isEmpty())
    {
        QMessageBox::warning(this, "Update Fail!", "ID and Name are required!");
        return;
    }

    if (!idRegex.match(id).hasMatch())
    {
        QMessageBox::warning(this, "Update Fail!", "ID must contain digits only!");
        return;
    }

    if (id.length() > 9 || !id.startsWith("08"))
    {
        QMessageBox::warning(this, "Update Fail!", "ID must start with 08 and <= 9 digits!");
        return;
    }

    if (!nameRegex.match(name).hasMatch())
    {
        QMessageBox::warning(this, "Update Fail!", "Name must contain letters only!");
        return;
    }

    if (major == "<<None>>" || !textRegex.match(major).hasMatch())
    {
        QMessageBox::warning(this, "Update Fail!", "Invalid major!");
        return;
    }

    if (gender == "<<None>>")
    {
        QMessageBox::warning(this, "Update Fail!", "Please select gender!");
        return;
    }

    if (className == "<<None>>")
    {
        QMessageBox::warning(this, "Update Fail!", "Please select class!");
        return;
    }

    for (int i = 0; i < students.size(); ++i)
    {
        if (i != row && students[i].id == id)
        {
            QMessageBox::warning(this, "Duplicate", "ID already exists!");
            return;
        }
    }

    QString oldAvatar = students[row].avatarPath;
    students[row] = Student(
        id,
        name,
        age,
        gender,
        major,
        className,
        oldAvatar
    );

    modified = true;

    emit dataChanged();

    refreshTable();
}

void StudentManagerWidget::deleteStudent()
{
    int row = table->currentRow();
    if (row < 0) return;

    if (QMessageBox::question(this, "Confirm Delete", "Delete this student?")
        != QMessageBox::Yes) return;

    students.removeAt(row);

    modified = true;

    emit dataChanged();

    refreshTable();
}


void StudentManagerWidget::searchStudent(const QString& text)
{
    for (int i = 0; i < table->rowCount(); ++i)
    {
        bool match =
            table->item(i, 1)->text().contains(text, Qt::CaseInsensitive);

        table->setRowHidden(i, !match);
    }
}

void StudentManagerWidget::updateClassByMajor(const QString& major)
{
    classBox->clear();
    classBox->addItem("<<None>>");

    if (majorClassMap.contains(major))
    {
        classBox->addItems(majorClassMap[major]);
    }
}

void StudentManagerWidget::saveToFile()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save", "", "*.json");
    if (fileName.isEmpty()) return;

    QJsonArray arr;

    for (const Student& s : students)
        arr.append(s.toJson());

    QFile file(fileName);

    if (!file.open(QIODevice::WriteOnly)) return;

    file.write(QJsonDocument(arr).toJson());
}


void StudentManagerWidget::loadFromFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open", "", "*.json");
    if (fileName.isEmpty()) return;

    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly)) return;

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    QJsonArray arr = doc.array();

    students.clear();

    for (auto v : arr)
        students.append(Student::fromJson(v.toObject()));

    refreshTable();
}
