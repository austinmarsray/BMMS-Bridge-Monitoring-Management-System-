#ifndef USER_H
#define USER_H

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
#include <QColor>
#include <QSpinBox>
#include <QHeaderView>
#include <QDoubleSpinBox>
#include "sensor.h"
#include "cpset.h"

class user : public QWidget
{
    Q_OBJECT
public:
    explicit user(QWidget *parent = 0);

signals:
    void back_signal_user();
public slots:
    void back_send();
private:
    QDialog *dialog;
    QMenuBar *menu;
    QTableWidget *table;
    QWidget *blank;
    QDoubleSpinBox *spinbox1;
    QDoubleSpinBox *spinbox2;
    QLabel *label1;
    QLabel *label2;
};

#endif // USER_H
