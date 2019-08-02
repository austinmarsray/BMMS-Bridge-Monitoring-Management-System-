#include "mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{

    this->setWindowFlags(Qt::FramelessWindowHint);
    //this->resize(QSize(800,400));
    this->setFixedSize(600,390);
    this->setWindowTitle(QString("登录界面"));
    this->setStyleSheet("MainWindow{border-image:url(://picture/1.jpg)}");


    account=new QLabel(this);

    account->setText(QString("桥梁管理系统"));
    account->resize(500,100);
    account->move(170,50);
    QFont t=account->font();
    t.setPointSize(32);
    account->setFont(t);

    account=new QLabel(this);
    account->setText(QString("账号"));
    account->move(140,150);


    password=new QLabel(this);
    password->setText(QString("密码"));
    password->move(140,200);

    account_in=new QLineEdit(this);
    account_in->setMaximumWidth(280);
    account_in->setMinimumWidth(280);
    account_in->move(170,150);
    account_in->setClearButtonEnabled(true);

    password_in=new QLineEdit(this);
    password_in->setMaximumWidth(280);
    password_in->setMinimumWidth(280);
    password_in->move(170,200);
    password_in->setEchoMode(QLineEdit::Password);
    password_in->setClearButtonEnabled(true);

    QStringList list;
    list<<"administrator"<<"user";
    QCompleter *completer=new QCompleter(list,account_in);
    account_in->setCompleter(completer);

    login=new QPushButton(this);
    //QPixmap denglu(":/picture/login.png");
    //denglu.scaled();
    login->resize(60,60);
    login->move(160,275);
    login->setStyleSheet("border-image:url(:/picture/login.png)");
    login->setFocus();
    login->setShortcut( QKeySequence::InsertParagraphSeparator );
    login->setShortcut(Qt::Key_Enter);
    login->setShortcut(Qt::Key_Return);

    connect(login,SIGNAL(clicked(bool)),this,SLOT(Login()));


    connect(&adminwindow,SIGNAL(back_signal_admin()),this,SLOT(back_admin()));
    connect(&userwindow,SIGNAL(back_signal_user()),this,SLOT(back_user()));


    qui=new QPushButton(this);
    qui->resize(60,60);
    qui->move(350,275);
    qui->setStyleSheet("border-image:url(:/picture/quit.png)");
    connect(qui,SIGNAL(clicked()),this,SLOT(Qui()));
}

void MainWindow::Qui()
{
    this->close();
}

void MainWindow::Login()
{
    QString account1("administrator");
    QString password1("123456789");
    QString account2("user");
    QString password2("987654321");

    QString a_tem;
    QString p_tem;

    a_tem=this->account_in->text();
    p_tem=this->password_in->text();

    if(a_tem!=account1 && a_tem!=account2)
    {
        QMessageBox *wrong;
        wrong=new QMessageBox(this);
        wrong->setWindowTitle(QString("错误"));
        wrong->setText(QString("账号或密码错误！"));
        wrong->show();
        account_in->clear();
        password_in->clear();
    }

    if(a_tem==account1 && p_tem!=password1)
    {
        QMessageBox *wrong;
        wrong=new QMessageBox(this);
        wrong->setWindowTitle(QString("错误"));
        wrong->setText(QString("账号或密码错误！"));
        wrong->show();
        password_in->clear();
    }

    if(a_tem==account2 && p_tem!=password2)
    {
        QMessageBox *wrong;
        wrong=new QMessageBox(this);
        wrong->setWindowTitle(QString("错误"));
        wrong->setText(QString("账号或密码错误！"));
        wrong->show();
        password_in->clear();
    }

    if(a_tem==account1 && p_tem==password1)
    {
        this->hide();
        adminwindow.show();
    }

    if(a_tem==account2 && p_tem==password2)
    {
        this->hide();
        userwindow.show();
    }

}

void MainWindow::back_admin()
{
    adminwindow.hide();
    this->show();
}

void MainWindow::back_user()
{
    userwindow.hide();
    this->show();
}

MainWindow::~MainWindow()
{

}
