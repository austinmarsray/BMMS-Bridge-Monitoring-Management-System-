#include "administrator.h"
#include <QDebug>


strainSet a;
displacementSet b;
deflectionSet c;
strain_cp_Set A;
displacement_cp_Set B;
deflection_cp_Set C;

administrator::administrator(QWidget *parent) : QWidget(parent)
{
    QFile file("://qss/gray.qss");
        file.open(QFile::ReadOnly);
        QTextStream filetext(&file);
        QString stylesheet = filetext.readAll();
        this->setStyleSheet(stylesheet);//设置样式表

    //管理员窗口设置
    this->setWindowTitle(QString("管理员界面"));
    //this ->setMaximumSize(1350,800);
    this->setMinimumSize(1000,800);

    //菜单栏
    menu=new QMenuBar(this);

    //一级菜单
    QAction *back=menu->addAction(QString("返回登陆界面"));
    QMenu *sensor_m=menu->addMenu(QString("传感器"));
    QMenu *cp_m=menu->addMenu(QString("监测点"));

    //二级菜单
    //传感器
    QAction *strain_s_a = sensor_m->addAction(QString("应变传感器"));
    QAction *dis_s_a = sensor_m->addAction(QString("位移传感器"));
    QAction *def_s_a = sensor_m->addAction(QString("桡度传感器"));
    //监测点
    QAction *strain_cp_a = cp_m->addAction(QString("应变监测点"));
    QAction *dis_cp_a = cp_m->addAction(QString("位移监测点"));
    QAction *def_cp_a = cp_m->addAction(QString("桡度监测点"));

    //显示区域
    blank=new QWidget(this);
    blank->move(0,25);
    blank->setAutoFillBackground(true);
    blank->setBackgroundRole(QPalette::Light);
    blank->setMaximumSize(1500,880);
    blank->setMinimumSize(1500,880);

    table=NULL;
    //功能
    connect(back,SIGNAL(triggered(bool)),this,SLOT(back_send()));
    //对传感器的操作
    connect(strain_s_a,&QAction::triggered,
            [=] ()
            {
                //功能按钮
                QPushButton *Modify=new QPushButton(blank);
                Modify->setText(QString("初始化/修改"));
                Modify->move(100,600);
                Modify->show();
                QPushButton *Show=new QPushButton(blank);
                Show->setText(QString("显示"));
                Show->move(250,600);
                Show->show();
                QPushButton *Search=new QPushButton(blank);
                Search->setText(QString("查找"));
                Search->move(400,600);
                Search->show();
                QPushButton *Delete=new QPushButton(blank);
                 Delete->setText(QString("删除"));
                 Delete->move(550,600);
                 Delete->show();
                 QPushButton *Add=new QPushButton(blank);
                 Add->setText(QString("添加"));
                 Add->move(700,600);
                 Add->show();

                delete table;
                table=new QTableWidget(10,8,blank);
                table->move(0,40);
                table->setMaximumSize(1105,500);
                table->setMinimumSize(1105,500);
                QStringList header;
                table->horizontalHeader()->setStretchLastSection(true);//自动延长

                header<<"传感器编号"<<"名称"<<"规格"<<"型号"<<"生产厂家"<<"生产日期"<<"传感器类型"<<"中心波长";
                table->setHorizontalHeaderLabels(header);
                table->show();

                //完成添加，删除，修改，查找功能
                connect(Add,&QPushButton::clicked,
                        [=]()
                        {
                            a.read_from_file();
                            delete table;
                            table=new QTableWidget(a.num+1,8,blank);
                            table->move(0,40);
                            table->setMaximumSize(1105,500);
                            table->setMinimumSize(1105,500);
                            table->horizontalHeader()->setStretchLastSection(true);//自动延长
                            QStringList header;
                            header<<"传感器编号"<<"名称"<<"规格"<<"型号"<<"生产厂家"<<"生产日期"<<"传感器类型"<<"中心波长";
                            table->setHorizontalHeaderLabels(header);
                            table->show();

                            for(int i=0;i<a.num;i++)
                            {
                                int j=0;
                                table->setItem(i,j++,new QTableWidgetItem(a.strain_s[i].get_identifier()));
                                table->setItem(i,j++,new QTableWidgetItem(a.strain_s[i].get_name()));
                                table->setItem(i,j++,new QTableWidgetItem(a.strain_s[i].get_specification()));
                                table->setItem(i,j++,new QTableWidgetItem(a.strain_s[i].get_version()));
                                table->setItem(i,j++,new QTableWidgetItem(a.strain_s[i].get_manufacturer()));
                                table->setItem(i,j++,new QTableWidgetItem(a.strain_s[i].get_produceDate()));
                                table->setItem(i,j++,new QTableWidgetItem(QString::number(a.strain_s[i].get_type())));
                                table->setItem(i,j++,new QTableWidgetItem(QString::number(a.strain_s[i].get_standardwavelength())));
                            }
                        }
                        );
                connect(Delete,&QPushButton::clicked,
                        [=]()
                        {
                                dialog=new QDialog;
                                dialog->setWindowTitle("删除");
                                dialog->resize(400,200);

                                QLabel *label;
                                label=new QLabel(dialog);
                                label->setText("请输入待删除传感器编号");

                                QLineEdit *lineEdit;
                                lineEdit=new QLineEdit(dialog);
                                lineEdit->resize(350,30);
                                lineEdit->move(20,50);

                                QPushButton *btn;
                                btn=new QPushButton(dialog);
                                btn->setText(QString("确定"));
                                btn->move(150,150);

                                connect(btn,&QPushButton::clicked,
                                        [=]()
                                        {
                                             QString tem;
                                             tem=lineEdit->text();
                                            a.read_from_file();
                                            if(a.delet(tem)==0)
                                            {
                                                QMessageBox::warning(NULL, "warning", "找不到该编号的传感器！", QMessageBox::Yes);
                                            }
                                            else
                                            {
                                                a.put_in_file();
                                                delete table;
                                                table=new QTableWidget(a.num,8,blank);
                                                table->move(0,40);
                                                table->setMaximumSize(1105,500);
                                                table->setMinimumSize(1105,500);
                                                QStringList header;
                                                table->horizontalHeader()->setStretchLastSection(true);//自动延长

                                                header<<"传感器编号"<<"名称"<<"规格"<<"型号"<<"生产厂家"<<"生产日期"<<"传感器类型"<<"中心波长";
                                                table->setHorizontalHeaderLabels(header);
                                                table->show();
                                                for(int i=0;i<a.num;i++)
                                                {
                                                    int j=0;
                                                    table->setItem(i,j++,new QTableWidgetItem(a.strain_s[i].get_identifier()));
                                                    table->setItem(i,j++,new QTableWidgetItem(a.strain_s[i].get_name()));
                                                    table->setItem(i,j++,new QTableWidgetItem(a.strain_s[i].get_specification()));
                                                    table->setItem(i,j++,new QTableWidgetItem(a.strain_s[i].get_version()));
                                                    table->setItem(i,j++,new QTableWidgetItem(a.strain_s[i].get_manufacturer()));
                                                    table->setItem(i,j++,new QTableWidgetItem(a.strain_s[i].get_produceDate()));
                                                    table->setItem(i,j++,new QTableWidgetItem(QString::number(a.strain_s[i].get_type())));
                                                    table->setItem(i,j++,new QTableWidgetItem(QString::number(a.strain_s[i].get_standardwavelength())));
                                                }
                                            }
                                            dialog->close();
                                        }
                                );
                                dialog->exec();
                        }
                         );
                connect(Search,&QPushButton::clicked,
                        [=]()
                        {
                            dialog=new QDialog;
                            dialog->setWindowTitle("查找");
                            dialog->resize(400,200);

                            QLabel *label;
                            label=new QLabel(dialog);
                            label->setText("请输入待查找传感器编号");

                            QLineEdit *lineEdit;
                            lineEdit=new QLineEdit(dialog);
                            lineEdit->resize(350,30);
                            lineEdit->move(20,50);

                            QPushButton *btn;
                            btn=new QPushButton(dialog);
                            btn->setText(QString("确定"));
                            btn->move(150,150);

                            connect(btn,&QPushButton::clicked,
                                    [=]()
                                    {
                                         QString tem;
                                         tem=lineEdit->text();
                                        a.read_from_file();
                                        if(a.search(tem)==-1)
                                        {
                                            QMessageBox::warning(NULL, "warning", "找不到该编号的传感器！", QMessageBox::Yes);
                                        }
                                        else
                                        {
                                            delete table;
                                            table=new QTableWidget(1,8,blank);
                                            table->move(0,40);
                                            table->setMaximumSize(1105,500);
                                            table->setMinimumSize(1105,500);
                                            QStringList header;
                                            table->horizontalHeader()->setStretchLastSection(true);//自动延长
                                            header<<"传感器编号"<<"名称"<<"规格"<<"型号"<<"生产厂家"<<"生产日期"<<"传感器类型"<<"中心波长";
                                            table->setHorizontalHeaderLabels(header);
                                            table->show();

                                            int j=0,i=a.search(tem);
                                            table->setItem(0,j++,new QTableWidgetItem(a.strain_s[i].get_identifier()));
                                            table->setItem(0,j++,new QTableWidgetItem(a.strain_s[i].get_name()));
                                            table->setItem(0,j++,new QTableWidgetItem(a.strain_s[i].get_specification()));
                                            table->setItem(0,j++,new QTableWidgetItem(a.strain_s[i].get_version()));
                                            table->setItem(0,j++,new QTableWidgetItem(a.strain_s[i].get_manufacturer()));
                                            table->setItem(0,j++,new QTableWidgetItem(a.strain_s[i].get_produceDate()));
                                            table->setItem(0,j++,new QTableWidgetItem(QString::number(a.strain_s[i].get_type())));
                                            table->setItem(0,j++,new QTableWidgetItem(QString::number(a.strain_s[i].get_standardwavelength())));
                                        }
                                        dialog->close();
                                    }
                            );
                            dialog->exec();
                    }
                        );
                connect(Modify,&QPushButton::clicked,
                        [=]()
                        {
                        a.num=0;
                        int row=table->rowCount();
                        for(int i=0;i<row;i++)                      //注意：如果有位置为空则程序会崩
                        {
                                a.num++;
                                int j=0;
                                a.strain_s[i].set_identifier(table->item(i,j++)->text());
                                a.strain_s[i].set_name(table->item(i,j++)->text());
                                a.strain_s[i].set_specification(table->item(i,j++)->text());
                                a.strain_s[i].set_version(table->item(i,j++)->text());
                                a.strain_s[i].set_manufacturer(table->item(i,j++)->text());
                                a.strain_s[i].set_produceDate(table->item(i,j++)->text());
                                a.strain_s[i].set_type(table->item(i,j++)->text().toInt());
                                a.strain_s[i].set_standardwavelength(table->item(i,j++)->text().toDouble());
                        }
                        a.put_in_file();
                        QMessageBox::warning(NULL,"提示","初始化/修改 成功！");
                        }
                        );
                connect(Show,&QPushButton::clicked,
                        [=]()
                        {
                            a.read_from_file();
                            delete table;
                            table=new QTableWidget(a.num,8,blank);
                            table->move(0,40);
                            table->setMaximumSize(1105,500);
                            table->setMinimumSize(1105,500);
                            QStringList header;
                            table->horizontalHeader()->setStretchLastSection(true);//自动延长

                            header<<"传感器编号"<<"名称"<<"规格"<<"型号"<<"生产厂家"<<"生产日期"<<"传感器类型"<<"中心波长";
                            table->setHorizontalHeaderLabels(header);
                            table->show();
                            for(int i=0;i<a.num;i++)
                            {
                                int j=0;
                                table->setItem(i,j++,new QTableWidgetItem(a.strain_s[i].get_identifier()));
                                table->setItem(i,j++,new QTableWidgetItem(a.strain_s[i].get_name()));
                                table->setItem(i,j++,new QTableWidgetItem(a.strain_s[i].get_specification()));
                                table->setItem(i,j++,new QTableWidgetItem(a.strain_s[i].get_version()));
                                table->setItem(i,j++,new QTableWidgetItem(a.strain_s[i].get_manufacturer()));
                                table->setItem(i,j++,new QTableWidgetItem(a.strain_s[i].get_produceDate()));
                                table->setItem(i,j++,new QTableWidgetItem(QString::number(a.strain_s[i].get_type())));
                                table->setItem(i,j++,new QTableWidgetItem(QString::number(a.strain_s[i].get_standardwavelength(),'f',5)));
                            }
                        }
                        );
            }
     );
    connect(dis_s_a,&QAction::triggered,
            [=]()
            {
                //功能按钮
                QPushButton *Modify=new QPushButton(blank);
                Modify->setText(QString("初始化/修改"));
                Modify->move(100,600);
                Modify->show();
                QPushButton *Show=new QPushButton(blank);
                Show->setText(QString("显示"));
                Show->move(250,600);
                Show->show();
                QPushButton *Search=new QPushButton(blank);
                Search->setText(QString("查找"));
                Search->move(400,600);
                Search->show();
                QPushButton *Delete=new QPushButton(blank);
                Delete->setText(QString("删除"));
                Delete->move(550,600);
                Delete->show();
                QPushButton *Add=new QPushButton(blank);
                Add->setText(QString("添加"));
                Add->move(700,600);
                Add->show();
                delete table;
                table=new QTableWidget(2,10,blank);
                table->move(0,40);
                table->setMaximumSize(1275,500);
                table->setMinimumSize(1275,500);
                QStringList header;
                table->horizontalHeader()->setStretchLastSection(true);//自动延长
                header<<"传感器编号"<<"名称"<<"规格"<<"型号"<<"生产厂家"<<"生产日期"<<"传感器类型"<<"初始数值"<<"最大值"<<"最小值";
                table->setHorizontalHeaderLabels(header);
                table->show();

                //完成添加，删除，修改，查找功能
                connect(Add,&QPushButton::clicked,
                        [=]()
                        {
                            b.read_from_file();
                            delete table;
                            table=new QTableWidget(b.num+1,10,blank);
                            table->move(0,40);
                            table->setMaximumSize(1275,500);
                            table->setMinimumSize(1275,500);
                            QStringList header;
                            table->horizontalHeader()->setStretchLastSection(true);//自动延长
                            header<<"传感器编号"<<"名称"<<"规格"<<"型号"<<"生产厂家"<<"生产日期"<<"传感器类型"<<"初始数值"<<"最大值"<<"最小值";
                            table->setHorizontalHeaderLabels(header);
                            table->show();

                            for(int i=0;i<b.num;i++)
                            {
                                int j=0;
                                table->setItem(i,j++,new QTableWidgetItem(b.dis_s[i].get_identifier()));
                                table->setItem(i,j++,new QTableWidgetItem(b.dis_s[i].get_name()));
                                table->setItem(i,j++,new QTableWidgetItem(b.dis_s[i].get_specification()));
                                table->setItem(i,j++,new QTableWidgetItem(b.dis_s[i].get_version()));
                                table->setItem(i,j++,new QTableWidgetItem(b.dis_s[i].get_manufacturer()));
                                table->setItem(i,j++,new QTableWidgetItem(b.dis_s[i].get_produceDate()));
                                table->setItem(i,j++,new QTableWidgetItem(QString::number(b.dis_s[i].get_type())));
                                table->setItem(i,j++,new QTableWidgetItem(QString::number(b.dis_s[i].get_init())));
                                table->setItem(i,j++,new QTableWidgetItem(QString::number(b.dis_s[i].get_upper_limit())));
                                table->setItem(i,j++,new QTableWidgetItem(QString::number(b.dis_s[i].get_lower_limit())));
                            }
                        }
                        );
                connect(Delete,&QPushButton::clicked,
                        [=]()
                        {
                                dialog=new QDialog;
                                dialog->setWindowTitle("删除");
                                dialog->resize(400,200);

                                QLabel *label;
                                label=new QLabel(dialog);
                                label->setText("请输入待删除传感器编号");

                                QLineEdit *lineEdit;
                                lineEdit=new QLineEdit(dialog);
                                lineEdit->resize(350,30);
                                lineEdit->move(20,50);

                                QPushButton *btn;
                                btn=new QPushButton(dialog);
                                btn->setText(QString("确定"));
                                btn->move(150,150);

                                connect(btn,&QPushButton::clicked,
                                        [=]()
                                        {
                                             QString tem;
                                             tem=lineEdit->text();
                                            b.read_from_file();
                                            if(b.delet(tem)==0)
                                            {
                                                QMessageBox::warning(NULL, "warning", "找不到该编号的传感器！", QMessageBox::Yes);
                                            }
                                            else
                                            {
                                                b.put_in_file();
                                                delete table;
                                                table=new QTableWidget(b.num,10,blank);
                                                table->move(0,40);
                                                table->setMaximumSize(1275,500);
                                                table->setMinimumSize(1275,500);
                                                QStringList header;
                                                table->horizontalHeader()->setStretchLastSection(true);//自动延长
                                                header<<"传感器编号"<<"名称"<<"规格"<<"型号"<<"生产厂家"<<"生产日期"<<"传感器类型"<<"初始数值"<<"最大值"<<"最小值";
                                                table->setHorizontalHeaderLabels(header);
                                                table->show();
                                                for(int i=0;i<b.num;i++)
                                                {
                                                    int j=0;
                                                    table->setItem(i,j++,new QTableWidgetItem(b.dis_s[i].get_identifier()));
                                                    table->setItem(i,j++,new QTableWidgetItem(b.dis_s[i].get_name()));
                                                    table->setItem(i,j++,new QTableWidgetItem(b.dis_s[i].get_specification()));
                                                    table->setItem(i,j++,new QTableWidgetItem(b.dis_s[i].get_version()));
                                                    table->setItem(i,j++,new QTableWidgetItem(b.dis_s[i].get_manufacturer()));
                                                    table->setItem(i,j++,new QTableWidgetItem(b.dis_s[i].get_produceDate()));
                                                    table->setItem(i,j++,new QTableWidgetItem(QString::number(b.dis_s[i].get_type())));
                                                    table->setItem(i,j++,new QTableWidgetItem(QString::number(b.dis_s[i].get_init())));
                                                    table->setItem(i,j++,new QTableWidgetItem(QString::number(b.dis_s[i].get_upper_limit())));
                                                    table->setItem(i,j++,new QTableWidgetItem(QString::number(b.dis_s[i].get_lower_limit())));
                                                }
                                            }
                                            dialog->close();
                                        }
                                );
                                dialog->exec();
                        }
                         );
                connect(Search,&QPushButton::clicked,
                        [=]()
                        {
                            dialog=new QDialog;
                            dialog->setWindowTitle("查找");
                            dialog->resize(400,200);

                            QLabel *label;
                            label=new QLabel(dialog);
                            label->setText("请输入待查找传感器编号");

                            QLineEdit *lineEdit;
                            lineEdit=new QLineEdit(dialog);
                            lineEdit->resize(350,30);
                            lineEdit->move(20,50);

                            QPushButton *btn;
                            btn=new QPushButton(dialog);
                            btn->setText(QString("确定"));
                            btn->move(150,150);

                            connect(btn,&QPushButton::clicked,
                                    [=]()
                                    {
                                         QString tem;
                                         tem=lineEdit->text();
                                        b.read_from_file();
                                        if(b.search(tem)==-1)
                                        {
                                            QMessageBox::warning(NULL, "warning", "找不到该编号的传感器！", QMessageBox::Yes);
                                        }
                                        else
                                        {
                                            delete table;
                                            table=new QTableWidget(1,10,blank);
                                            table->move(0,40);
                                            table->setMaximumSize(1275,500);
                                            table->setMinimumSize(1275,500);
                                            QStringList header;
                                            table->horizontalHeader()->setStretchLastSection(true);//自动延长
                                            header<<"传感器编号"<<"名称"<<"规格"<<"型号"<<"生产厂家"<<"生产日期"<<"传感器类型"<<"初始数值"<<"最大值"<<"最小值";
                                            table->setHorizontalHeaderLabels(header);
                                            table->show();

                                            int j=0,i=b.search(tem);
                                            table->setItem(0,j++,new QTableWidgetItem(b.dis_s[i].get_identifier()));
                                            table->setItem(0,j++,new QTableWidgetItem(b.dis_s[i].get_name()));
                                            table->setItem(0,j++,new QTableWidgetItem(b.dis_s[i].get_specification()));
                                            table->setItem(0,j++,new QTableWidgetItem(b.dis_s[i].get_version()));
                                            table->setItem(0,j++,new QTableWidgetItem(b.dis_s[i].get_manufacturer()));
                                            table->setItem(0,j++,new QTableWidgetItem(b.dis_s[i].get_produceDate()));
                                            table->setItem(0,j++,new QTableWidgetItem(QString::number(b.dis_s[i].get_type())));
                                            table->setItem(0,j++,new QTableWidgetItem(QString::number(b.dis_s[i].get_init())));
                                            table->setItem(0,j++,new QTableWidgetItem(QString::number(b.dis_s[i].get_upper_limit())));
                                            table->setItem(0,j++,new QTableWidgetItem(QString::number(b.dis_s[i].get_lower_limit())));
                                        }
                                        dialog->close();
                                    }
                            );
                            dialog->exec();
                    }
                        );
                connect(Modify,&QPushButton::clicked,
                        [=]()
                        {
                        b.num=0;
                        int row=table->rowCount();
                        for(int i=0;i<row;i++)                      //注意：如果有位置为空则程序会崩
                        {
                                b.num++;
                                int j=0;
                                b.dis_s[i].set_identifier(table->item(i,j++)->text());
                                b.dis_s[i].set_name(table->item(i,j++)->text());
                                b.dis_s[i].set_specification(table->item(i,j++)->text());
                                b.dis_s[i].set_version(table->item(i,j++)->text());
                                b.dis_s[i].set_manufacturer(table->item(i,j++)->text());
                                b.dis_s[i].set_produceDate(table->item(i,j++)->text());
                                b.dis_s[i].set_type(table->item(i,j++)->text().toInt());
                                b.dis_s[i].set_init(table->item(i,j++)->text().toDouble());
                                b.dis_s[i].set_upper_limit(table->item(i,j++)->text().toDouble());
                                b.dis_s[i].set_lower_limit(table->item(i,j++)->text().toDouble());
                        }
                        b.put_in_file();
                        QMessageBox::warning(NULL,"提示","初始化/修改 成功！");
                        }
                        );
                connect(Show,&QPushButton::clicked,
                        [=]()
                        {
                            b.read_from_file();
                            delete table;
                            table=new QTableWidget(b.num,10,blank);
                            table->move(0,40);
                            table->setMaximumSize(1275,500);
                            table->setMinimumSize(1275,500);
                            QStringList header;
                            table->horizontalHeader()->setStretchLastSection(true);//自动延长
                            header<<"传感器编号"<<"名称"<<"规格"<<"型号"<<"生产厂家"<<"生产日期"<<"传感器类型"<<"初始数值"<<"最大值"<<"最小值";
                            table->setHorizontalHeaderLabels(header);
                            table->show();
                            for(int i=0;i<b.num;i++)
                            {
                                int j=0;
                                table->setItem(i,j++,new QTableWidgetItem(b.dis_s[i].get_identifier()));
                                table->setItem(i,j++,new QTableWidgetItem(b.dis_s[i].get_name()));
                                table->setItem(i,j++,new QTableWidgetItem(b.dis_s[i].get_specification()));
                                table->setItem(i,j++,new QTableWidgetItem(b.dis_s[i].get_version()));
                                table->setItem(i,j++,new QTableWidgetItem(b.dis_s[i].get_manufacturer()));
                                table->setItem(i,j++,new QTableWidgetItem(b.dis_s[i].get_produceDate()));
                                table->setItem(i,j++,new QTableWidgetItem(QString::number(b.dis_s[i].get_type())));
                                table->setItem(i,j++,new QTableWidgetItem(QString::number(b.dis_s[i].get_init())));
                                table->setItem(i,j++,new QTableWidgetItem(QString::number(b.dis_s[i].get_upper_limit())));
                                table->setItem(i,j++,new QTableWidgetItem(QString::number(b.dis_s[i].get_lower_limit())));
                            }
                        }
                        );
            }
    );
    connect(def_s_a,&QAction::triggered,//挠度传感器界面
            [=]()
            {
                //功能按钮
                QPushButton *Modify=new QPushButton(blank);
                Modify->setText(QString("初始化/修改"));
                Modify->move(100,600);
                Modify->show();
                QPushButton *Show=new QPushButton(blank);
                Show->setText(QString("显示"));
                Show->move(250,600);
                Show->show();
                QPushButton *Search=new QPushButton(blank);
                Search->setText(QString("查找"));
                Search->move(400,600);
                Search->show();
                QPushButton *Delete=new QPushButton(blank);
                 Delete->setText(QString("删除"));
                 Delete->move(550,600);
                 Delete->show();
                 QPushButton *Add=new QPushButton(blank);
                 Add->setText(QString("添加"));
                 Add->move(700,600);
                 Add->show();
                 delete table;
                table=new QTableWidget(10,8,blank);
                table->move(0,40);
                table->setMaximumSize(1105,500);
                table->setMinimumSize(1105,500);
                QStringList header;
                table->horizontalHeader()->setStretchLastSection(true);//自动延长
                header<<"传感器编号"<<"名称"<<"规格"<<"型号"<<"生产厂家"<<"生产日期"<<"传感器类型"<<"单位";
                table->setHorizontalHeaderLabels(header);
                table->show();

                //完成添加，删除，修改，查找功能
                //增加
                connect(Add,&QPushButton::clicked,
                        [=]()
                        {
                            c.read_from_file();
                            delete table;
                            table=new QTableWidget(c.num+1,8,blank);
                            table->move(0,40);
                            table->setMaximumSize(1105,500);
                            table->setMinimumSize(1105,500);
                            QStringList header;
                            table->horizontalHeader()->setStretchLastSection(true);//自动延长
                            header<<"传感器编号"<<"名称"<<"规格"<<"型号"<<"生产厂家"<<"生产日期"<<"传感器类型"<<"单位";
                            table->setHorizontalHeaderLabels(header);
                            table->show();

                            for(int i=0;i<c.num;i++)
                            {
                                int j=0;
                                table->setItem(i,j++,new QTableWidgetItem(c.def_s[i].get_identifier()));
                                table->setItem(i,j++,new QTableWidgetItem(c.def_s[i].get_name()));
                                table->setItem(i,j++,new QTableWidgetItem(c.def_s[i].get_specification()));
                                table->setItem(i,j++,new QTableWidgetItem(c.def_s[i].get_version()));
                                table->setItem(i,j++,new QTableWidgetItem(c.def_s[i].get_manufacturer()));
                                table->setItem(i,j++,new QTableWidgetItem(c.def_s[i].get_produceDate()));
                                table->setItem(i,j++,new QTableWidgetItem(QString::number(c.def_s[i].get_type())));
                                table->setItem(i,j++,new QTableWidgetItem(c.def_s[i].get_unit()));
                            }
                        }
                        );
                //删除
                connect(Delete,&QPushButton::clicked,
                        [=]()
                        {
                                dialog=new QDialog;
                                dialog->setWindowTitle("删除");
                                dialog->resize(400,200);

                                QLabel *label;
                                label=new QLabel(dialog);
                                label->setText("请输入待删除传感器编号");

                                QLineEdit *lineEdit;
                                lineEdit=new QLineEdit(dialog);
                                lineEdit->resize(350,30);
                                lineEdit->move(20,50);

                                QPushButton *btn;
                                btn=new QPushButton(dialog);
                                btn->setText(QString("确定"));
                                btn->move(150,150);

                                connect(btn,&QPushButton::clicked,
                                        [=]()
                                        {
                                             QString tem;
                                             tem=lineEdit->text();
                                            c.read_from_file();
                                            if(c.delet(tem)==0)
                                            {
                                                QMessageBox::warning(NULL, "warning", "找不到该编号的传感器！", QMessageBox::Yes);
                                            }
                                            else
                                            {
                                                c.put_in_file();
                                                delete table;
                                                table=new QTableWidget(c.num,8,blank);
                                                table->move(0,40);
                                                table->setMaximumSize(1105,500);
                                                table->setMinimumSize(1105,500);
                                                QStringList header;
                                                table->horizontalHeader()->setStretchLastSection(true);//自动延长
                                                header<<"传感器编号"<<"名称"<<"规格"<<"型号"<<"生产厂家"<<"生产日期"<<"传感器类型"<<"单位";
                                                table->setHorizontalHeaderLabels(header);
                                                table->show();
                                                for(int i=0;i<c.num;i++)
                                                {
                                                    int j=0;
                                                    table->setItem(i,j++,new QTableWidgetItem(c.def_s[i].get_identifier()));
                                                    table->setItem(i,j++,new QTableWidgetItem(c.def_s[i].get_name()));
                                                    table->setItem(i,j++,new QTableWidgetItem(c.def_s[i].get_specification()));
                                                    table->setItem(i,j++,new QTableWidgetItem(c.def_s[i].get_version()));
                                                    table->setItem(i,j++,new QTableWidgetItem(c.def_s[i].get_manufacturer()));
                                                    table->setItem(i,j++,new QTableWidgetItem(c.def_s[i].get_produceDate()));
                                                    table->setItem(i,j++,new QTableWidgetItem(QString::number(c.def_s[i].get_type())));
                                                    table->setItem(i,j++,new QTableWidgetItem(c.def_s[i].get_unit()));
                                                }
                                            }
                                            dialog->close();
                                        }
                                );
                                dialog->exec();
                        }
                         );
                //查找
                connect(Search,&QPushButton::clicked,
                        [=]()
                        {
                            dialog=new QDialog;
                            dialog->setWindowTitle("查找");
                            dialog->resize(400,200);

                            QLabel *label;
                            label=new QLabel(dialog);
                            label->setText("请输入待查找传感器编号");

                            QLineEdit *lineEdit;
                            lineEdit=new QLineEdit(dialog);
                            lineEdit->resize(350,30);
                            lineEdit->move(20,50);

                            QPushButton *btn;
                            btn=new QPushButton(dialog);
                            btn->setText(QString("确定"));
                            btn->move(150,150);

                            connect(btn,&QPushButton::clicked,
                                    [=]()
                                    {
                                         QString tem;
                                         tem=lineEdit->text();
                                        c.read_from_file();
                                        if(c.search(tem)==-1)
                                        {
                                            QMessageBox::warning(NULL, "warning", "找不到该编号的传感器！", QMessageBox::Yes);
                                        }
                                        else
                                        {
                                            delete table;
                                            table=new QTableWidget(1,8,blank);
                                            table->move(0,40);
                                            table->setMaximumSize(1105,500);
                                            table->setMinimumSize(1105,500);
                                            QStringList header;
                                            table->horizontalHeader()->setStretchLastSection(true);//自动延长
                                            header<<"传感器编号"<<"名称"<<"规格"<<"型号"<<"生产厂家"<<"生产日期"<<"传感器类型"<<"单位";
                                            table->setHorizontalHeaderLabels(header);
                                            table->show();

                                            int j=0,i=c.search(tem);
                                            table->setItem(0,j++,new QTableWidgetItem(c.def_s[i].get_identifier()));
                                            table->setItem(0,j++,new QTableWidgetItem(c.def_s[i].get_name()));
                                            table->setItem(0,j++,new QTableWidgetItem(c.def_s[i].get_specification()));
                                            table->setItem(0,j++,new QTableWidgetItem(c.def_s[i].get_version()));
                                            table->setItem(0,j++,new QTableWidgetItem(c.def_s[i].get_manufacturer()));
                                            table->setItem(0,j++,new QTableWidgetItem(c.def_s[i].get_produceDate()));
                                            table->setItem(0,j++,new QTableWidgetItem(QString::number(c.def_s[i].get_type())));
                                            table->setItem(i,j++,new QTableWidgetItem(c.def_s[i].get_unit()));
                                        }
                                        dialog->close();
                                    }
                            );
                            dialog->exec();
                    }
                        );
                //修改
                connect(Modify,&QPushButton::clicked,
                        [=]()
                        {
                        c.num=0;
                        int row=table->rowCount();
                        for(int i=0;i<row;i++)                      //注意：如果有位置为空则程序会崩
                        {
                                c.num++;
                                int j=0;
                                c.def_s[i].set_identifier(table->item(i,j++)->text());
                                c.def_s[i].set_name(table->item(i,j++)->text());
                                c.def_s[i].set_specification(table->item(i,j++)->text());
                                c.def_s[i].set_version(table->item(i,j++)->text());
                                c.def_s[i].set_manufacturer(table->item(i,j++)->text());
                                c.def_s[i].set_produceDate(table->item(i,j++)->text());
                                c.def_s[i].set_type(table->item(i,j++)->text().toInt());
                                c.def_s[i].set_unit(table->item(i,j++)->text());
                        }
                        c.put_in_file();
                        QMessageBox::warning(NULL,"提示","初始化/修改 成功！");
                        }
                        );
                //显示
                connect(Show,&QPushButton::clicked,
                        [=]()
                        {
                            c.read_from_file();
                            delete table;
                            table=new QTableWidget(c.num,8,blank);
                            table->move(0,40);
                            table->setMaximumSize(1105,500);
                            table->setMinimumSize(1105,500);
                            QStringList header;
                            table->horizontalHeader()->setStretchLastSection(true);//自动延长
                            header<<"传感器编号"<<"名称"<<"规格"<<"型号"<<"生产厂家"<<"生产日期"<<"传感器类型"<<"单位";
                            table->setHorizontalHeaderLabels(header);
                            table->show();
                            for(int i=0;i<c.num;i++)
                            {
                                int j=0;
                                table->setItem(i,j++,new QTableWidgetItem(c.def_s[i].get_identifier()));
                                table->setItem(i,j++,new QTableWidgetItem(c.def_s[i].get_name()));
                                table->setItem(i,j++,new QTableWidgetItem(c.def_s[i].get_specification()));
                                table->setItem(i,j++,new QTableWidgetItem(c.def_s[i].get_version()));
                                table->setItem(i,j++,new QTableWidgetItem(c.def_s[i].get_manufacturer()));
                                table->setItem(i,j++,new QTableWidgetItem(c.def_s[i].get_produceDate()));
                                table->setItem(i,j++,new QTableWidgetItem(QString::number(c.def_s[i].get_type())));
                                table->setItem(i,j++,new QTableWidgetItem(c.def_s[i].get_unit()));
                            }
                        }
                        );
            }
     );

    //对监测点的操作
    connect(strain_cp_a,&QAction::triggered,
            [=]()
            {
                //功能按钮
                QPushButton *Modify=new QPushButton(blank);
                Modify->setText(QString("初始化/修改"));
                Modify->move(100,600);
                Modify->show();
                QPushButton *Show=new QPushButton(blank);
                Show->setText(QString("显示"));
                Show->move(250,600);
                Show->show();
                QPushButton *Search=new QPushButton(blank);
                Search->setText(QString("查找"));
                Search->move(400,600);
                Search->show();
                QPushButton *Delete=new QPushButton(blank);
                 Delete->setText(QString("删除"));
                 Delete->move(550,600);
                 Delete->show();
                 QPushButton *Add=new QPushButton(blank);
                 Add->setText(QString("添加"));
                 Add->move(700,600);
                 Add->show();

                delete table;
                table=new QTableWidget(10,5,blank);
                table->move(0,40);
                table->setMaximumSize(850,500);
                table->setMinimumSize(850,500);
                QStringList header;
                table->horizontalHeader()->setStretchLastSection(true);//自动延长
                header<<"监测点编号"<<"监测点名称"<<"监测点位置"<<"监测点类型"<<"传感器";
                table->setHorizontalHeaderLabels(header);
                table->show();

                //完成添加，删除，修改，查找功能
                connect(Add,&QPushButton::clicked,
                        [=]()
                        {
                            A.read_from_file();
                            delete table;
                            table=new QTableWidget(A.num+1,5,blank);
                            table->move(0,40);
                            table->setMaximumSize(850,500);
                            table->setMinimumSize(850,500);
                            QStringList header;
                            table->horizontalHeader()->setStretchLastSection(true);//自动延长
                            header<<"监测点编号"<<"监测点名称"<<"监测点位置"<<"监测点类型"<<"传感器";
                            table->setHorizontalHeaderLabels(header);
                            table->show();

                            for(int i=0;i<A.num;i++)
                            {
                                int j=0;
                                table->setItem(i,j++,new QTableWidgetItem(A.strain_cps[i].get_identifier()));
                                table->setItem(i,j++,new QTableWidgetItem(A.strain_cps[i].get_name()));
                                table->setItem(i,j++,new QTableWidgetItem(A.strain_cps[i].get_location()));
                                table->setItem(i,j++,new QTableWidgetItem(QString::number(A.strain_cps[i].get_type())));
                                table->setItem(i,j++,new QTableWidgetItem(A.strain_cps[i].get_Asensor().get_identifier()));
                            }
                        }
                        );
                connect(Delete,&QPushButton::clicked,
                        [=]()
                        {
                                dialog=new QDialog;
                                dialog->setWindowTitle("删除");
                                dialog->resize(400,200);

                                QLabel *label;
                                label=new QLabel(dialog);
                                label->setText("请输入待删除监测点编号");

                                QLineEdit *lineEdit;
                                lineEdit=new QLineEdit(dialog);
                                lineEdit->resize(350,30);
                                lineEdit->move(20,50);

                                QPushButton *btn;
                                btn=new QPushButton(dialog);
                                btn->setText(QString("确定"));
                                btn->move(150,150);

                                connect(btn,&QPushButton::clicked,
                                        [=]()
                                        {
                                             QString tem;
                                             tem=lineEdit->text();
                                            A.read_from_file();
                                            if(A.delet(tem)==0)
                                            {
                                                QMessageBox::warning(NULL, "warning", "找不到该编号的监测点！", QMessageBox::Yes);
                                            }
                                            else
                                            {
                                                A.put_in_file();
                                                delete table;
                                                table=new QTableWidget(A.num,5,blank);
                                                table->move(0,40);
                                                table->setMaximumSize(850,500);
                                                table->setMinimumSize(850,500);
                                                QStringList header;
                                                table->horizontalHeader()->setStretchLastSection(true);//自动延长
                                                header<<"监测点编号"<<"监测点名称"<<"监测点位置"<<"监测点类型"<<"传感器";
                                                table->setHorizontalHeaderLabels(header);
                                                table->show();
                                                for(int i=0;i<A.num;i++)
                                                {
                                                    int j=0;
                                                    table->setItem(i,j++,new QTableWidgetItem(A.strain_cps[i].get_identifier()));
                                                    table->setItem(i,j++,new QTableWidgetItem(A.strain_cps[i].get_name()));
                                                    table->setItem(i,j++,new QTableWidgetItem(A.strain_cps[i].get_location()));
                                                    table->setItem(i,j++,new QTableWidgetItem(QString::number(A.strain_cps[i].get_type())));
                                                    table->setItem(i,j++,new QTableWidgetItem(A.strain_cps[i].get_Asensor().get_identifier()));
                                                }
                                            }
                                            dialog->close();
                                        }
                                );
                                dialog->exec();
                        }
                         );
                connect(Search,&QPushButton::clicked,
                        [=]()
                        {
                            dialog=new QDialog;
                            dialog->setWindowTitle("查找");
                            dialog->resize(400,200);

                            QLabel *label;
                            label=new QLabel(dialog);
                            label->setText("请输入待查找监测点编号");

                            QLineEdit *lineEdit;
                            lineEdit=new QLineEdit(dialog);
                            lineEdit->resize(350,30);
                            lineEdit->move(20,50);

                            QPushButton *btn;
                            btn=new QPushButton(dialog);
                            btn->setText(QString("确定"));
                            btn->move(150,150);

                            connect(btn,&QPushButton::clicked,
                                    [=]()
                                    {
                                         QString tem;
                                         tem=lineEdit->text();
                                        A.read_from_file();
                                        if(A.search(tem)==-1)
                                        {
                                            QMessageBox::warning(NULL, "warning", "找不到该编号的监测点！", QMessageBox::Yes);
                                        }
                                        else
                                        {
                                            delete table;
                                            table=new QTableWidget(1,5,blank);
                                            table->move(0,40);
                                            table->setMaximumSize(850,500);
                                            table->setMinimumSize(850,500);
                                            QStringList header;
                                            table->horizontalHeader()->setStretchLastSection(true);//自动延长
                                            header<<"监测点编号"<<"监测点名称"<<"监测点位置"<<"监测点类型"<<"传感器";
                                            table->setHorizontalHeaderLabels(header);
                                            table->show();

                                            int j=0,i=A.search(tem);
                                            table->setItem(0,j++,new QTableWidgetItem(A.strain_cps[i].get_identifier()));
                                            table->setItem(0,j++,new QTableWidgetItem(A.strain_cps[i].get_name()));
                                            table->setItem(0,j++,new QTableWidgetItem(A.strain_cps[i].get_location()));
                                            table->setItem(0,j++,new QTableWidgetItem(QString::number(A.strain_cps[i].get_type())));
                                            table->setItem(0,j,new QTableWidgetItem(A.strain_cps[i].get_Asensor().get_identifier()));
                                        }
                                        dialog->close();
                                    }
                            );
                            dialog->exec();
                    }
                        );
                connect(Modify,&QPushButton::clicked,
                        [=]()
                        {
                            A.num=0;
                            int row=table->rowCount();
                            a.read_from_file();                            //便于后续获取传感器信息
                            int flag=0;
                            for(int i=0;i<row;i++)                      //注意：如果有位置为空则程序会崩
                            {
                                    A.num++;
                                    int j=0;
                                    A.strain_cps[i].set_identifier(table->item(i,j++)->text());
                                    A.strain_cps[i].set_name(table->item(i,j++)->text());
                                    A.strain_cps[i].set_location(table->item(i,j++)->text());
                                    A.strain_cps[i].set_type(table->item(i,j++)->text().toInt());

                                    //传感器验证
                                    if(a.search(table->item(i,j)->text())==-1)
                                    {
                                        QMessageBox::warning(NULL,"出错","无此传感器！");
                                        table->setItem(i,j,new QTableWidgetItem(QString(" ")));
                                        flag=1;
                                        break;
                                    }
                                    else
                                    {
                                         A.strain_cps[i].set_Asensor(a.get_sensor(a.search(table->item(i,j)->text())));
                                    }
                             }
                            if(flag==0)
                            {
                                //读入原始数据
                                A.read_pri_data();
                                //计算得到需求数据
                                for(int k=0;k<A.num;k++)
                                {
                                    A.strain_cps[k].calculate();
                                }
                                //存储需求数据
                                A.write_ans_data();
                                //测点基础数据写入文件
                               A.put_in_file();
                               QMessageBox::warning(NULL,"提示","初始化/修改 成功！");
                            }
                        }
                        );
                connect(Show,&QPushButton::clicked,
                        [=]()
                        {
                            A.read_from_file();
                            delete table;
                            table=new QTableWidget(A.num,5,blank);
//                            table->setEditTriggers(QAbstractItemView::NoEditTriggers);
                            table->move(0,40);
                            table->setMaximumSize(850,500);
                            table->setMinimumSize(850,500);
                            QStringList header;
                            table->horizontalHeader()->setStretchLastSection(true);//自动延长
                            header<<"监测点编号"<<"监测点名称"<<"监测点位置"<<"监测点类型"<<"传感器";
                            table->setHorizontalHeaderLabels(header);
                            table->show();
                            for(int i=0;i<A.num;i++)
                            {
                                int j=0;
                                table->setItem(i,j++,new QTableWidgetItem(A.strain_cps[i].get_identifier()));
                                table->setItem(i,j++,new QTableWidgetItem(A.strain_cps[i].get_name()));
                                table->setItem(i,j++,new QTableWidgetItem(A.strain_cps[i].get_location()));
                                table->setItem(i,j++,new QTableWidgetItem(QString::number(A.strain_cps[i].get_type())));
                                table->setItem(i,j++,new QTableWidgetItem(A.strain_cps[i].get_Asensor().get_identifier()));
                            }

                            connect(table,&QTableWidget::itemDoubleClicked,
                                    [=]()
                                    {
                                        int Row=table->currentItem()->row();
                                        int Column=table->currentItem()->column();
                                        if(Column==4)
                                        {
                                            QString tem=table->item(Row,Column)->text();
                                            strainSensor Tem=a.get_sensor(a.search(tem));
                                            QDialog *dialog_tem=new QDialog;                    //出错
                                            qDebug()<<"1";
                                            dialog_tem->setWindowTitle("传感器信息");
                                            dialog_tem->resize(1100,200);
                                            QTableWidget *table_tem=new QTableWidget(1,8,dialog_tem);
                                            table_tem->setEditTriggers(QAbstractItemView::NoEditTriggers);
                                            table_tem->setMaximumSize(1100,200);
                                            table_tem->setMinimumSize(1100,200);
                                            QStringList header;
                                            header<<"传感器编号"<<"名称"<<"规格"<<"型号"<<"生产厂家"<<"生产日期"<<"传感器类型"<<"中心波长";
                                            table_tem->setHorizontalHeaderLabels(header);
                                            dialog_tem->show();

                                            int j=0;
                                            table_tem->setItem(0,j++,new QTableWidgetItem(Tem.get_identifier()));
                                            table_tem->setItem(0,j++,new QTableWidgetItem(Tem.get_name()));
                                            table_tem->setItem(0,j++,new QTableWidgetItem(Tem.get_specification()));
                                            table_tem->setItem(0,j++,new QTableWidgetItem(Tem.get_version()));
                                            table_tem->setItem(0,j++,new QTableWidgetItem(Tem.get_manufacturer()));
                                            table_tem->setItem(0,j++,new QTableWidgetItem(Tem.get_produceDate()));
                                            table_tem->setItem(0,j++,new QTableWidgetItem(QString::number(Tem.get_type())));
                                            table_tem->setItem(0,j++,new QTableWidgetItem(QString::number(Tem.get_standardwavelength())));

                                            dialog_tem->exec();
                                        }
                                    }
                            );
                        }
                        );
            }
    );
    connect(dis_cp_a,&QAction::triggered,
            [=]()
            {
                //功能按钮
                QPushButton *Modify=new QPushButton(blank);
                Modify->setText(QString("初始化/修改"));
                Modify->move(100,600);
                Modify->show();
                QPushButton *Show=new QPushButton(blank);
                Show->setText(QString("显示"));
                Show->move(250,600);
                Show->show();
                QPushButton *Search=new QPushButton(blank);
                Search->setText(QString("查找"));
                Search->move(400,600);
                Search->show();
                QPushButton *Delete=new QPushButton(blank);
                Delete->setText(QString("删除"));
                Delete->move(550,600);
                Delete->show();
                QPushButton *Add=new QPushButton(blank);
                Add->setText(QString("添加"));
                Add->move(700,600);
                Add->show();

                delete table;
                table=new QTableWidget(2,5,blank);
                table->move(0,40);
                table->setMaximumSize(850,500);
                table->setMinimumSize(850,500);
                QStringList header;
                table->horizontalHeader()->setStretchLastSection(true);//自动延长
                header<<"监测点编号"<<"监测点名称"<<"监测点位置"<<"监测点类型"<<"传感器";
                table->setHorizontalHeaderLabels(header);
                table->show();

                //完成添加，删除，修改，查找功能
                connect(Add,&QPushButton::clicked,
                        [=]()
                        {
                            B.read_from_file();
                            delete table;
                            table=new QTableWidget(B.num+1,5,blank);
                            table->move(0,40);
                            table->setMaximumSize(850,500);
                            table->setMinimumSize(850,500);
                            QStringList header;
                            table->horizontalHeader()->setStretchLastSection(true);//自动延长
                            header<<"监测点编号"<<"监测点名称"<<"监测点位置"<<"监测点类型"<<"传感器";
                            table->setHorizontalHeaderLabels(header);
                            table->show();

                            for(int i=0;i<B.num;i++)
                            {
                                int j=0;
                                table->setItem(i,j++,new QTableWidgetItem(B.displacement_cps[i].get_identifier()));
                                table->setItem(i,j++,new QTableWidgetItem(B.displacement_cps[i].get_name()));
                                table->setItem(i,j++,new QTableWidgetItem(B.displacement_cps[i].get_location()));
                                table->setItem(i,j++,new QTableWidgetItem(QString::number(B.displacement_cps[i].get_type())));
                                table->setItem(i,j++,new QTableWidgetItem(B.displacement_cps[i].get_Asensor().get_identifier()));
                            }
                        }
                        );
                connect(Delete,&QPushButton::clicked,
                        [=]()
                        {
                            dialog=new QDialog;
                            dialog->setWindowTitle("删除");
                            dialog->resize(400,200);

                            QLabel *label;
                            label=new QLabel(dialog);
                            label->setText("请输入待删除监测点编号");

                            QLineEdit *lineEdit;
                            lineEdit=new QLineEdit(dialog);
                            lineEdit->resize(350,30);
                            lineEdit->move(20,50);

                            QPushButton *btn;
                            btn=new QPushButton(dialog);
                            btn->setText(QString("确定"));
                            btn->move(150,150);

                            connect(btn,&QPushButton::clicked,
                                    [=]()
                                    {
                                        QString tem;
                                        tem=lineEdit->text();
                                        B.read_from_file();
                                        if(B.delet(tem)==0)
                                        {
                                            QMessageBox::warning(NULL, "warning", "找不到该编号的监测点！", QMessageBox::Yes);
                                        }
                                        else
                                        {
                                            B.put_in_file();
                                            delete table;
                                            table=new QTableWidget(B.num,5,blank);
                                            table->move(0,40);
                                            table->setMaximumSize(850,500);
                                            table->setMinimumSize(850,500);
                                            QStringList header;
                                            table->horizontalHeader()->setStretchLastSection(true);//自动延长
                                            header<<"监测点编号"<<"监测点名称"<<"监测点位置"<<"监测点类型"<<"传感器";
                                            table->setHorizontalHeaderLabels(header);
                                            table->show();
                                            for(int i=0;i<B.num;i++)
                                            {
                                                int j=0;
                                                table->setItem(i,j++,new QTableWidgetItem(B.displacement_cps[i].get_identifier()));
                                                table->setItem(i,j++,new QTableWidgetItem(B.displacement_cps[i].get_name()));
                                                table->setItem(i,j++,new QTableWidgetItem(B.displacement_cps[i].get_location()));
                                                table->setItem(i,j++,new QTableWidgetItem(QString::number(B.displacement_cps[i].get_type())));
                                                table->setItem(i,j++,new QTableWidgetItem(B.displacement_cps[i].get_Asensor().get_identifier()));
                                            }
                                        }
                                        dialog->close();
                                    }
                                );
                            dialog->exec();
                        }
                        );
                connect(Search,&QPushButton::clicked,
                        [=]()
                        {
                            dialog=new QDialog;
                            dialog->setWindowTitle("查找");
                            dialog->resize(400,200);

                            QLabel *label;
                            label=new QLabel(dialog);
                            label->setText("请输入待查找监测点编号");

                            QLineEdit *lineEdit;
                            lineEdit=new QLineEdit(dialog);
                            lineEdit->resize(350,30);
                            lineEdit->move(20,50);

                            QPushButton *btn;
                            btn=new QPushButton(dialog);
                            btn->setText(QString("确定"));
                            btn->move(150,150);

                            connect(btn,&QPushButton::clicked,
                                    [=]()
                                    {
                                        QString tem;
                                        tem=lineEdit->text();
                                        B.read_from_file();
                                        if(B.search(tem)==-1)
                                        {
                                            QMessageBox::warning(NULL, "warning", "找不到该编号的监测点！", QMessageBox::Yes);
                                        }
                                        else
                                        {
                                            delete table;
                                            table=new QTableWidget(1,5,blank);
                                            table->move(0,40);
                                            table->setMaximumSize(850,500);
                                            table->setMinimumSize(850,500);
                                            QStringList header;
                                            table->horizontalHeader()->setStretchLastSection(true);//自动延长
                                            header<<"监测点编号"<<"监测点名称"<<"监测点位置"<<"监测点类型"<<"传感器";
                                            table->setHorizontalHeaderLabels(header);
                                            table->show();

                                            int j=0,i=B.search(tem);
                                            table->setItem(0,j++,new QTableWidgetItem(B.displacement_cps[i].get_identifier()));
                                            table->setItem(0,j++,new QTableWidgetItem(B.displacement_cps[i].get_name()));
                                            table->setItem(0,j++,new QTableWidgetItem(B.displacement_cps[i].get_location()));
                                            table->setItem(0,j++,new QTableWidgetItem(QString::number(B.displacement_cps[i].get_type())));
                                            table->setItem(0,j,new QTableWidgetItem(B.displacement_cps[i].get_Asensor().get_identifier()));
                                        }
                                        dialog->close();
                                    }
                                    );
                                dialog->exec();
                            }
                        );
                connect(Modify,&QPushButton::clicked,
                        [=]()
                        {
                            B.num=0;
                            int row=table->rowCount();
                            b.read_from_file();                            //便于后续获取传感器信息
                            int flag=0;
                            for(int i=0;i<row;i++)                      //注意：如果有位置为空则程序会崩
                            {
                                B.num++;
                                int j=0;
                                B.displacement_cps[i].set_identifier(table->item(i,j++)->text());
                                B.displacement_cps[i].set_name(table->item(i,j++)->text());
                                B.displacement_cps[i].set_location(table->item(i,j++)->text());
                                B.displacement_cps[i].set_type(table->item(i,j++)->text().toInt());

                                //传感器验证
                                if(b.search(table->item(i,j)->text())==-1)
                                {
                                    QMessageBox::warning(NULL,"出错","无此传感器！");
                                    table->setItem(i,j,new QTableWidgetItem(QString(" ")));
                                    flag=1;
                                    break;
                                }
                                else
                                {
                                    B.displacement_cps[i].set_Asensor(b.get_sensor(b.search(table->item(i,j)->text())));
                                }
                            }
                            if(flag==0)
                            {
                                //读入原始数据
                                B.read_pri_data();
                                //计算得到需求数据
                                for(int k=0;k<B.num;k++)
                                {
                                    B.displacement_cps[k].calculate();
                                }
                                //存储需求数据
                                B.write_ans_data();
                                //测点基础数据写入文件
                                B.put_in_file();
                                QMessageBox::warning(NULL,"提示","初始化/修改 成功！");
                            }
                        }
                        );
                connect(Show,&QPushButton::clicked,
                        [=]()
                        {
                            B.read_from_file();
                            delete table;
                            table=new QTableWidget(B.num,5,blank);
                            table->move(0,40);
                            table->setMaximumSize(850,500);
                            table->setMinimumSize(850,500);
                            QStringList header;
                            table->horizontalHeader()->setStretchLastSection(true);//自动延长
                            header<<"监测点编号"<<"监测点名称"<<"监测点位置"<<"监测点类型"<<"传感器";
                            table->setHorizontalHeaderLabels(header);
                            table->show();
                            for(int i=0;i<B.num;i++)
                            {
                                int j=0;
                                table->setItem(i,j++,new QTableWidgetItem(B.displacement_cps[i].get_identifier()));
                                table->setItem(i,j++,new QTableWidgetItem(B.displacement_cps[i].get_name()));
                                table->setItem(i,j++,new QTableWidgetItem(B.displacement_cps[i].get_location()));
                                table->setItem(i,j++,new QTableWidgetItem(QString::number(B.displacement_cps[i].get_type())));
                                table->setItem(i,j++,new QTableWidgetItem(B.displacement_cps[i].get_Asensor().get_identifier()));
                            }
                        }
                        );
            }
    );
    connect(def_cp_a,&QAction::triggered,//挠度监测点
                [=]()
                {
                    //功能按钮
                    QPushButton *Modify=new QPushButton(blank);
                    Modify->setText(QString("初始化/修改"));
                    Modify->move(100,600);
                    Modify->show();
                    QPushButton *Show=new QPushButton(blank);
                    Show->setText(QString("显示"));
                    Show->move(250,600);
                    Show->show();
                    QPushButton *Search=new QPushButton(blank);
                    Search->setText(QString("查找"));
                    Search->move(400,600);
                    Search->show();
                    QPushButton *Delete=new QPushButton(blank);
                     Delete->setText(QString("删除"));
                     Delete->move(550,600);
                     Delete->show();
                     QPushButton *Add=new QPushButton(blank);
                     Add->setText(QString("添加"));
                     Add->move(700,600);
                     Add->show();

                    delete table;
                    table=new QTableWidget(14,5,blank);
                    table->move(0,40);
                    table->setMaximumSize(850,500);
                    table->setMinimumSize(850,500);
                    QStringList header;
                    table->horizontalHeader()->setStretchLastSection(true);//自动延长
                    header<<"监测点编号"<<"监测点名称"<<"监测点位置"<<"监测点类型"<<"传感器";
                    table->setHorizontalHeaderLabels(header);
                    table->show();

                    connect(Add,&QPushButton::clicked,//增加
                            [=]()
                            {
                                C.read_from_file();
                                delete table;
                                table=new QTableWidget(C.num+1,5,blank);
                                table->move(0,40);
                                table->setMaximumSize(850,500);
                                table->setMinimumSize(850,500);
                                QStringList header;
                                table->horizontalHeader()->setStretchLastSection(true);//自动延长
                                header<<"监测点编号"<<"监测点名称"<<"监测点位置"<<"监测点类型"<<"传感器";
                                table->setHorizontalHeaderLabels(header);
                                table->show();

                                for(int i=0;i<C.num;i++)
                                {
                                    int j=0;
                                    table->setItem(i,j++,new QTableWidgetItem(C.deflection_cps[i].get_identifier()));
                                    table->setItem(i,j++,new QTableWidgetItem(C.deflection_cps[i].get_name()));
                                    table->setItem(i,j++,new QTableWidgetItem(C.deflection_cps[i].get_location()));
                                    table->setItem(i,j++,new QTableWidgetItem(QString::number(C.deflection_cps[i].get_type())));

                                    table->setItem(i,j++,new QTableWidgetItem(C.deflection_cps[i].get_Asensor().get_identifier()));
                                }
                            }
                            );

                    connect(Delete,&QPushButton::clicked,//删除
                            [=]()
                            {
                                    dialog=new QDialog;
                                    dialog->setWindowTitle("删除");
                                    dialog->resize(400,200);

                                    QLabel *label;
                                    label=new QLabel(dialog);
                                    label->setText("请输入待删除监测点编号");

                                    QLineEdit *lineEdit;
                                    lineEdit=new QLineEdit(dialog);
                                    lineEdit->resize(350,30);
                                    lineEdit->move(20,50);

                                    QPushButton *btn;
                                    btn=new QPushButton(dialog);
                                    btn->setText(QString("确定"));
                                    btn->move(150,150);

                                    connect(btn,&QPushButton::clicked,
                                            [=]()
                                            {
                                                 QString tem;
                                                 tem=lineEdit->text();
                                                C.read_from_file();
                                                if(C.delet(tem)==0)
                                                {
                                                    QMessageBox::warning(NULL, "warning", "找不到该编号的监测点！", QMessageBox::Yes);
                                                }
                                                else
                                                {
                                                    C.put_in_file();
                                                    delete table;
                                                    table=new QTableWidget(C.num,5,blank);
                                                    table->move(0,40);
                                                    table->setMaximumSize(850,500);
                                                    table->setMinimumSize(850,500);
                                                    QStringList header;
                                                    table->horizontalHeader()->setStretchLastSection(true);//自动延长
                                                    header<<"监测点编号"<<"监测点名称"<<"监测点位置"<<"监测点类型"<<"传感器";
                                                    table->setHorizontalHeaderLabels(header);
                                                    table->show();
                                                    for(int i=0;i<C.num;i++)
                                                    {
                                                        int j=0;
                                                        table->setItem(i,j++,new QTableWidgetItem(C.deflection_cps[i].get_identifier()));
                                                        table->setItem(i,j++,new QTableWidgetItem(C.deflection_cps[i].get_name()));
                                                        table->setItem(i,j++,new QTableWidgetItem(C.deflection_cps[i].get_location()));
                                                        table->setItem(i,j++,new QTableWidgetItem(QString::number(C.deflection_cps[i].get_type())));

                                                        table->setItem(i,j++,new QTableWidgetItem(C.deflection_cps[i].get_Asensor().get_identifier()));
                                                    }
                                                }
                                                dialog->close();
                                            }
                                    );
                                    dialog->exec();
                            }
                             );

                    connect(Search,&QPushButton::clicked,//查找
                            [=]()
                            {
                                dialog=new QDialog;
                                dialog->setWindowTitle("查找");
                                dialog->resize(400,200);

                                QLabel *label;
                                label=new QLabel(dialog);
                                label->setText("请输入待查找监测点编号");

                                QLineEdit *lineEdit;
                                lineEdit=new QLineEdit(dialog);
                                lineEdit->resize(350,30);
                                lineEdit->move(20,50);

                                QPushButton *btn;
                                btn=new QPushButton(dialog);
                                btn->setText(QString("确定"));
                                btn->move(150,150);

                                connect(btn,&QPushButton::clicked,
                                        [=]()
                                        {
                                             QString tem;
                                             tem=lineEdit->text();
                                            C.read_from_file();
                                            if(C.search(tem)==-1)
                                            {
                                                QMessageBox::warning(NULL, "warning", "找不到该编号的监测点！", QMessageBox::Yes);
                                            }
                                            else
                                            {
                                                delete table;
                                                table=new QTableWidget(1,5,blank);
                                                table->move(0,40);
                                                table->setMaximumSize(850,500);
                                                table->setMinimumSize(850,500);
                                                QStringList header;
                                                table->horizontalHeader()->setStretchLastSection(true);//自动延长
                                                header<<"监测点编号"<<"监测点名称"<<"监测点位置"<<"监测点类型"<<"传感器";
                                                table->setHorizontalHeaderLabels(header);
                                                table->show();

                                                int j=0,i=C.search(tem);
                                                table->setItem(0,j++,new QTableWidgetItem(C.deflection_cps[i].get_identifier()));
                                                table->setItem(0,j++,new QTableWidgetItem(C.deflection_cps[i].get_name()));
                                                table->setItem(0,j++,new QTableWidgetItem(C.deflection_cps[i].get_location()));
                                                table->setItem(0,j++,new QTableWidgetItem(QString::number(C.deflection_cps[i].get_type())));

                                                table->setItem(0,j,new QTableWidgetItem(C.deflection_cps[i].get_Asensor().get_identifier()));
                                            }
                                            dialog->close();
                                        }
                                );
                                dialog->exec();
                        }
                            );
                    connect(Modify,&QPushButton::clicked,
                            [=]()
                            {
                                C.num=0;
                                int row=table->rowCount();
                                c.read_from_file();                            //便于后续获取传感器信息
                                for(int i=0;i<row;i++)                      //注意：如果有位置为空则程序会崩
                                {
                                        C.num++;
                                        int j=0;
                                        C.deflection_cps[i].set_identifier(table->item(i,j++)->text());
                                        C.deflection_cps[i].set_name(table->item(i,j++)->text());
                                        C.deflection_cps[i].set_location(table->item(i,j++)->text());
                                        C.deflection_cps[i].set_type(table->item(i,j++)->text().toInt());


                                        //传感器验证
                                        if(c.search(table->item(i,j)->text())==-1)
                                        {
                                            QMessageBox::warning(NULL,"出错","无此传感器！");
                                            table->setItem(i,j,new QTableWidgetItem(QString(" ")));
                                            break;
                                        }
                                        else
                                        {
                                             C.deflection_cps[i].set_Asensor(c.get_sensor(c.search(table->item(i,j)->text())));
                                        }
                                 }
                                //读入原始数据
                                C.read_pri_data();

                                //计算得到需求数据
                                for(int k=0;k<C.num;k++)
                                {
                                    C.deflection_cps[k].calculate(C.deflection_cps[0].get_pri_data());
                                }

                                //存储需求数据
                                C.write_ans_data();

                                //测点基础数据写入文件
                               C.put_in_file();

                                QMessageBox::warning(NULL,"提示","初始化/修改 成功！");
                            }
                            );
                    connect(Show,&QPushButton::clicked,
                            [=]()
                            {
                                C.read_from_file();
                                delete table;
                                table=new QTableWidget(C.num,5,blank);
                                table->resizeRowsToContents();
                                table->move(0,40);
                                table->setMaximumSize(850,500);
                                table->setMinimumSize(850,500);
                                QStringList header;
                                table->horizontalHeader()->setStretchLastSection(true);//自动延长
                                header<<"监测点编号"<<"监测点名称"<<"监测点位置"<<"监测点类型"<<"传感器";
                                table->setHorizontalHeaderLabels(header);
                                table->show();
                                for(int i=0;i<C.num;i++)
                                {
                                    int j=0;
                                    table->setItem(i,j++,new QTableWidgetItem(C.deflection_cps[i].get_identifier()));
                                    table->setItem(i,j++,new QTableWidgetItem(C.deflection_cps[i].get_name()));
                                    table->setItem(i,j++,new QTableWidgetItem(C.deflection_cps[i].get_location()));
                                    table->setItem(i,j++,new QTableWidgetItem(QString::number(C.deflection_cps[i].get_type())));

                                    table->setItem(i,j++,new QTableWidgetItem(C.deflection_cps[i].get_Asensor().get_identifier()));
                                }
                            }
                            );
                }
   );
}


void administrator::back_send()
{
    emit back_signal_admin();
}
