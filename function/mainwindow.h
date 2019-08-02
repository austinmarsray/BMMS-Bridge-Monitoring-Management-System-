#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QMessageBox>
#include <QCompleter>
#include <QStringList>
#include "user.h"
#include "administrator.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    QPushButton *login;
    QPushButton *qui;
    QLineEdit *account_in;
    QLineEdit *password_in;
    QLabel *account;
    QLabel *password;
    user userwindow;
    administrator adminwindow;
private slots:
    void Login();
    void Qui();
    void back_admin();
    void back_user();
};

#endif // MAINWINDOW_H
