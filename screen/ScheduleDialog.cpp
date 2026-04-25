#include "ScheduleDialog.h"
#include <QVBoxLayout>
#include <QTableWidget>
#include <QHeaderView>
#include <QTableWidgetItem>

ScheduleDialog::ScheduleDialog(QWidget* parent)
    : QDialog(parent), table(nullptr)
{
    setWindowTitle("Courses");
    resize(500, 300);
}

void ScheduleDialog::setSchedule(const QVector<Course>& courses)
{
    if (table) {
        delete table; 
    }

    table = new QTableWidget(this);
    table->setColumnCount(4);
    table->setHorizontalHeaderLabels({ "ID", "Name", "Teacher", "Class" });
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    table->setRowCount(courses.size());
    for (int i = 0; i < courses.size(); i++) {
        const Course& c = courses[i];
        table->setItem(i, 0, new QTableWidgetItem(c.id));
        table->setItem(i, 1, new QTableWidgetItem(c.name));
        table->setItem(i, 2, new QTableWidgetItem(c.teacher));
        table->setItem(i, 3, new QTableWidgetItem(c.className));
    }

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(table);
    setLayout(layout);
}
