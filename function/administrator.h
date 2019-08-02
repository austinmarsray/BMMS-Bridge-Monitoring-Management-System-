#ifndef ADMINISTRATOR_H
#define ADMINISTRATOR_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QMenuBar>
#include <QMessageBox>
#include <QLineEdit>
#include <QMenu>
#include <QDialog>
#include <QAction>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QStringList>
#include <QDataStream>
#include <QTextStream>
#include <QFile>
#include "sensor.h"
#include "cpset.h"

class administrator : public QWidget
{
    Q_OBJECT
public:
    explicit administrator(QWidget *parent = 0);
signals:
    void back_signal_admin();
public slots:
    void back_send();
private:
    QDialog *dialog;
    QMenuBar *menu;
    QTableWidget *table;
    QWidget *blank;
};

#endif // ADMINISTRATOR_H
