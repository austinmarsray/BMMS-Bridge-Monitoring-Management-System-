#include "user.h"

strainSet a0;
displacementSet b0;
deflectionSet c0;
strain_cp_Set A0;
displacement_cp_Set B0;
deflection_cp_Set C0;

user::user(QWidget *parent) : QWidget(parent)
{
    QFile file("://qss/gray.qss");
        file.open(QFile::ReadOnly);
        QTextStream filetext(&file);
        QString stylesheet = filetext.readAll();
        this->setStyleSheet(stylesheet);//设置样式表
    this->setWindowTitle(QString("用户界面"));
    this ->setMaximumSize(1600,900);
    this->setMinimumSize(1600,900);

    //菜单栏
    menu=new QMenuBar(this);

    QAction *back=menu->addAction(QString("返回登陆界面"));
    //监测点
    QAction *strain_cp_a = menu->addAction(QString("应变监测点"));
    QAction *dis_cp_a = menu->addAction(QString("位移监测点"));
    QAction *def_cp_a = menu->addAction(QString("桡度监测点"));

    //显示区域
    blank=new QWidget(this);
    blank->move(0,25);
    blank->setAutoFillBackground(true);
    blank->setBackgroundRole(QPalette::Light);
    blank->setMaximumSize(1600,880);
    blank->setMinimumSize(1600,880);

    table=NULL;
    spinbox1=NULL;
    spinbox2=NULL;
    label1=NULL;
    label2=NULL;

    connect(back,SIGNAL(triggered(bool)),this,SLOT(back_send()));

    connect(strain_cp_a,&QAction::triggered,
            [=]()
            {
                a0.read_from_file();
                A0.read_from_file();

                delete spinbox1;
                delete spinbox2;
                delete label1;
                delete label2;
                spinbox1=new QDoubleSpinBox(blank);
                spinbox2=new QDoubleSpinBox(blank);
                label1=new QLabel("警戒下线",blank);
                label2=new QLabel("警戒上线",blank);
                label1->move(0,10);
                label1->setFixedSize(100,40);
                label1->setScaledContents(true);
                label1->show();
                spinbox1->move(100,15);
                spinbox1->setFixedSize(100,30);
                spinbox1->setRange(0, 3.50);
                spinbox1->setDecimals(2);
                spinbox1->setSingleStep(0.1);
                spinbox1->show();
                label2->move(250,10);
                label2->setFixedSize(100,40);
                label2->setScaledContents(true);
                label2->show();
                spinbox2->move(350,15);
                spinbox2->setFixedSize(100,30);
                spinbox2->setRange(4, 6);
                spinbox2->setDecimals(2);
                spinbox2->setSingleStep(0.1);
                spinbox2->show();

                delete table;
                table=new QTableWidget(A0.strain_cps[0].get_ans_data().num+4,A0.num,blank);
                table->setEditTriggers(QAbstractItemView::NoEditTriggers);
                table->move(0,60);
                table->setMaximumSize(1600,800);
                table->setMinimumSize(1600,800);
                QStringList header;
                for(int i=0;i<A0.num;i++)
                {
                    header<<A0.strain_cps[i].get_name();
                }
                table->setHorizontalHeaderLabels(header);

                header.clear();
                header<<"监测点编号"<<"监测点位置"<<"类型"<<"传感器";
                for(int i=0;i<A0.strain_cps[0].get_ans_data().num;i++)
                {
                    header<<A0.strain_cps[0].get_ans_data().Strain_data_s[i].date+" "+A0.strain_cps[0].get_ans_data().Strain_data_s[i].time;
                }
                table->setVerticalHeaderLabels(header);
                table->show();

                for(int i=0;i<A0.num;i++)
                {
                    int j=0;
                    table->setItem(j++,i,new QTableWidgetItem(A0.strain_cps[i].get_identifier()));
                    table->setItem(j++,i,new QTableWidgetItem(A0.strain_cps[i].get_location()));
                    table->setItem(j++,i,new QTableWidgetItem(QString::number(A0.strain_cps[i].get_type())));
                    table->setItem(j++,i,new QTableWidgetItem(A0.strain_cps[i].get_Asensor().get_identifier()));
                    for(int k=0;k<A0.strain_cps[i].get_ans_data().num;k++)
                    {
                        QTableWidgetItem *item=new QTableWidgetItem(QString::number(A0.strain_cps[i].get_ans_data().Strain_data_s[k].ans,'f',5));
                        table->setItem(j++,i,item);
                    }
                }

                connect(table,&QTableWidget::itemDoubleClicked,
                        [=]()
                        {
                            int Row=table->currentItem()->row();
                            int Column=table->currentItem()->column();
                            if(Row==3)
                            {
                                QString tem=table->item(Row,Column)->text();
                                strainSensor Tem=a0.get_sensor(a0.search(tem));
                                QDialog *dialog=new QDialog;
                                dialog->setWindowTitle("传感器信息");
                                dialog->resize(1100,200);
                                QTableWidget *table_tem=new QTableWidget(1,8,dialog);
                                table_tem->setEditTriggers(QAbstractItemView::NoEditTriggers);
                                table_tem->setMaximumSize(1100,200);
                                table_tem->setMinimumSize(1100,200);
                                QStringList header;
                                header<<"传感器编号"<<"名称"<<"规格"<<"型号"<<"生产厂家"<<"生产日期"<<"传感器类型"<<"中心波长";
                                table_tem->setHorizontalHeaderLabels(header);
                                dialog->show();

                                int j=0;
                                table_tem->setItem(0,j++,new QTableWidgetItem(Tem.get_identifier()));
                                table_tem->setItem(0,j++,new QTableWidgetItem(Tem.get_name()));
                                table_tem->setItem(0,j++,new QTableWidgetItem(Tem.get_specification()));
                                table_tem->setItem(0,j++,new QTableWidgetItem(Tem.get_version()));
                                table_tem->setItem(0,j++,new QTableWidgetItem(Tem.get_manufacturer()));
                                table_tem->setItem(0,j++,new QTableWidgetItem(Tem.get_produceDate()));
                                table_tem->setItem(0,j++,new QTableWidgetItem(QString::number(Tem.get_type())));
                                table_tem->setItem(0,j++,new QTableWidgetItem(QString::number(Tem.get_standardwavelength())));

                                dialog->exec();
                            }
                        }
                );
                connect(spinbox1,static_cast<void(QDoubleSpinBox::*)(const QString &)>(&QDoubleSpinBox::valueChanged),
                        [=]()
                        {
                            double lowerLine=spinbox1->text().toDouble();
                            double upperLine=spinbox2->text().toDouble();
                            for(int i=0;i<A0.num;i++)
                            {
                                int j=0;
                                table->setItem(j++,i,new QTableWidgetItem(A0.strain_cps[i].get_identifier()));
                                table->setItem(j++,i,new QTableWidgetItem(A0.strain_cps[i].get_location()));
                                table->setItem(j++,i,new QTableWidgetItem(QString::number(A0.strain_cps[i].get_type())));
                                table->setItem(j++,i,new QTableWidgetItem(A0.strain_cps[i].get_Asensor().get_identifier()));
                                for(int k=0;k<A0.strain_cps[i].get_ans_data().num;k++)
                                {
                                    QTableWidgetItem *item=new QTableWidgetItem(QString::number(A0.strain_cps[i].get_ans_data().Strain_data_s[k].ans,'f',5));
                                    if(A0.strain_cps[i].get_ans_data().Strain_data_s[k].ans>=upperLine || A0.strain_cps[i].get_ans_data().Strain_data_s[k].ans<=lowerLine)
                                    {
                                        item->setTextColor(QColor(255,0,0));
                                    }
                                    table->setItem(j++,i,item);
                                }
                            }
                        }
                );
                connect(spinbox2,static_cast<void(QDoubleSpinBox::*)(const QString &)>(&QDoubleSpinBox::valueChanged),
                        [=]()
                        {
                            double lowerLine=spinbox1->text().toDouble();
                            double upperLine=spinbox2->text().toDouble();
                            for(int i=0;i<A0.num;i++)
                            {
                                int j=0;
                                table->setItem(j++,i,new QTableWidgetItem(A0.strain_cps[i].get_identifier()));
                                table->setItem(j++,i,new QTableWidgetItem(A0.strain_cps[i].get_location()));
                                table->setItem(j++,i,new QTableWidgetItem(QString::number(A0.strain_cps[i].get_type())));
                                table->setItem(j++,i,new QTableWidgetItem(A0.strain_cps[i].get_Asensor().get_identifier()));
                                for(int k=0;k<A0.strain_cps[i].get_ans_data().num;k++)
                                {
                                    QTableWidgetItem *item=new QTableWidgetItem(QString::number(A0.strain_cps[i].get_ans_data().Strain_data_s[k].ans,'f',5));
                                    if(A0.strain_cps[i].get_ans_data().Strain_data_s[k].ans>=upperLine || A0.strain_cps[i].get_ans_data().Strain_data_s[k].ans<=lowerLine)
                                    {
                                        item->setTextColor(QColor(255,0,0));
                                    }
                                    table->setItem(j++,i,item);
                                }
                            }
                        }
                );
           }
    );
     connect(dis_cp_a,&QAction::triggered,
    [=]()
    {
        b0.read_from_file();
        B0.read_from_file();

        delete spinbox1;
        delete spinbox2;
        delete label1;
        delete label2;
        spinbox2=NULL;
        label2=NULL;
        spinbox1=new QDoubleSpinBox(blank);
        label1=new QLabel("正常范围半径",blank);
        label1->move(0,10);
        label1->setFixedSize(100,40);
        label1->setScaledContents(true);
        label1->show();
        spinbox1->move(100,15);
        spinbox1->setFixedSize(100,30);
        spinbox1->setRange(0.5, 2);
        spinbox1->setDecimals(2);
        spinbox1->setSingleStep(0.1);
        spinbox1->show();

        delete table;
        table=new QTableWidget(B0.displacement_cps[0].get_ans_data().num+5,B0.num,blank);
        table->setEditTriggers(QAbstractItemView::NoEditTriggers);
        table->move(0,60);
        table->setMaximumSize(1600,800);
        table->setMinimumSize(1600,800);
        QStringList header;
        for(int i=0;i<B0.num;i++)
        {
            header<<B0.displacement_cps[i].get_name();
        }
        table->setHorizontalHeaderLabels(header);

        header.clear();
        header<<"监测点编号"<<"监测点位置"<<"类型"<<"传感器"<<"单位";
        for(int i=0;i<B0.displacement_cps[0].get_ans_data().num;i++)
        {
            header<<B0.displacement_cps[0].get_ans_data().Dis_data_s[i].date+" "+B0.displacement_cps[0].get_ans_data().Dis_data_s[i].time;
        }
        table->setVerticalHeaderLabels(header);
        table->show();

        for(int i=0;i<B0.num;i++)
        {
            int j=0;
            table->setItem(j++,i,new QTableWidgetItem(B0.displacement_cps[i].get_identifier()));
            table->setItem(j++,i,new QTableWidgetItem(B0.displacement_cps[i].get_location()));
            table->setItem(j++,i,new QTableWidgetItem(QString::number(B0.displacement_cps[i].get_type())));
            table->setItem(j++,i,new QTableWidgetItem(B0.displacement_cps[i].get_Asensor().get_identifier()));
            table->setItem(j++,i,new QTableWidgetItem(B0.displacement_cps[i].get_ans_data().Dis_data_s[0].unit));
            for(int k=0;k<B0.displacement_cps[i].get_ans_data().num;k++)
            {
                QTableWidgetItem *item=new QTableWidgetItem(QString::number(B0.displacement_cps[i].get_ans_data().Dis_data_s[k].dis_ans,'f',5));
                table->setItem(j++,i,item);
            }
        }

        connect(table,&QTableWidget::itemDoubleClicked,
                [=]()
                {
                    int Row=table->currentItem()->row();
                    int Column=table->currentItem()->column();
                    if(Row==3)
                    {
                        QString tem=table->item(Row,Column)->text();
                        displacementSensor Tem=b0.get_sensor(b0.search(tem));
                        QDialog *dialog=new QDialog;
                        dialog->setWindowTitle("传感器信息");
                        dialog->resize(1100,200);
                        QTableWidget *table_tem=new QTableWidget(1,10,dialog);
                        table_tem->setEditTriggers(QAbstractItemView::NoEditTriggers);
                        table_tem->setMaximumSize(1100,200);
                        table_tem->setMinimumSize(1100,200);
                        QStringList header;
                        header<<"传感器编号"<<"名称"<<"规格"<<"型号"<<"生产厂家"<<"生产日期"<<"传感器类型"<<"初始数值"<<"最大值"<<"最小值";
                        table_tem->setHorizontalHeaderLabels(header);
                        dialog->show();

                        int j=0;
                        table_tem->setItem(0,j++,new QTableWidgetItem(Tem.get_identifier()));
                        table_tem->setItem(0,j++,new QTableWidgetItem(Tem.get_name()));
                        table_tem->setItem(0,j++,new QTableWidgetItem(Tem.get_specification()));
                        table_tem->setItem(0,j++,new QTableWidgetItem(Tem.get_version()));
                        table_tem->setItem(0,j++,new QTableWidgetItem(Tem.get_manufacturer()));
                        table_tem->setItem(0,j++,new QTableWidgetItem(Tem.get_produceDate()));
                        table_tem->setItem(0,j++,new QTableWidgetItem(QString::number(Tem.get_type())));
                        table_tem->setItem(0,j++,new QTableWidgetItem(QString::number(Tem.get_init())));
                        table_tem->setItem(0,j++,new QTableWidgetItem(QString::number(Tem.get_upper_limit())));
                        table_tem->setItem(0,j++,new QTableWidgetItem(QString::number(Tem.get_lower_limit())));

                        dialog->exec();
                    }
                }
        );
        connect(spinbox1,static_cast<void(QDoubleSpinBox::*)(const QString &)>(&QDoubleSpinBox::valueChanged),
                [=]()
                {
                    double limit=spinbox1->text().toDouble();
                    for(int i=0;i<B0.num;i++)
                    {
                        int j=0;
                        table->setItem(j++,i,new QTableWidgetItem(B0.displacement_cps[i].get_identifier()));
                        table->setItem(j++,i,new QTableWidgetItem(B0.displacement_cps[i].get_location()));
                        table->setItem(j++,i,new QTableWidgetItem(QString::number(B0.displacement_cps[i].get_type())));
                        table->setItem(j++,i,new QTableWidgetItem(B0.displacement_cps[i].get_Asensor().get_identifier()));
                        for(int k=0;k<B0.displacement_cps[i].get_ans_data().num;k++)
                        {
                            QTableWidgetItem *item=new QTableWidgetItem(QString::number(B0.displacement_cps[i].get_ans_data().Dis_data_s[k].dis_ans,'f',5));
                            if(fabs(B0.displacement_cps[i].get_ans_data().Dis_data_s[k].dis_ans)>=limit)
                            {
                                item->setTextColor(QColor(255,0,0));
                            }
                            table->setItem(j++,i,item);
                        }
                    }
                }
        );
   }
);
    connect(def_cp_a,&QAction::triggered,
            [=]()
            {
                c0.read_from_file();
                C0.read_from_file();

                delete spinbox1;
                delete spinbox2;
                delete label1;
                delete label2;
                delete table;
                spinbox1=NULL;
                spinbox2=NULL;
                label1=NULL;
                label2=NULL;
                table=new QTableWidget(C0.deflection_cps[0].get_ans_data().num+5,C0.num,blank);
                table->setEditTriggers(QAbstractItemView::NoEditTriggers);
                table->move(0,60);
                table->setMaximumSize(1600,800);
                table->setMinimumSize(1600,800);
                QStringList header;
                for(int i=0;i<C0.num;i++)
                {
                    header<<C0.deflection_cps[i].get_name();
                }
                table->setHorizontalHeaderLabels(header);

                header.clear();
                header<<"监测点编号"<<"监测点位置"<<"类型"<<"传感器"<<"单位";
                for(int i=0;i<C0.deflection_cps[0].get_ans_data().num;i++)
                {
                    header<<C0.deflection_cps[0].get_ans_data().Deflecton_data_s[i].date+" "+C0.deflection_cps[0].get_ans_data().Deflecton_data_s[i].time;
                }
                table->setVerticalHeaderLabels(header);
                table->show();

                for(int i=0;i<C0.num;i++)
                {
                    int j=0;
                    table->setItem(j++,i,new QTableWidgetItem(C0.deflection_cps[i].get_identifier()));
                    table->setItem(j++,i,new QTableWidgetItem(C0.deflection_cps[i].get_location()));
                    table->setItem(j++,i,new QTableWidgetItem(QString::number(C0.deflection_cps[i].get_type())));
                    table->setItem(j++,i,new QTableWidgetItem(C0.deflection_cps[i].get_Asensor().get_identifier()));
                    table->setItem(j++,i,new QTableWidgetItem(C0.deflection_cps[i].get_ans_data().Deflecton_data_s[0].unit));
                    for(int k=0;k<C0.deflection_cps[i].get_ans_data().num;k++)
                    {
                        QTableWidgetItem *item=new QTableWidgetItem(QString::number(C0.deflection_cps[i].get_ans_data().Deflecton_data_s[k].def_ans,'f',5));
                        table->setItem(j++,i,item);
                    }
                }

                connect(table,&QTableWidget::itemDoubleClicked,
                        [=]()
                        {
                            int Row=table->currentItem()->row();
                            int Column=table->currentItem()->column();
                            if(Row==3)
                            {
                                QString tem=table->item(Row,Column)->text();
                                deflectionSensor Tem=c0.get_sensor(c0.search(tem));
                                QDialog *dialog=new QDialog;
                                dialog->setWindowTitle("传感器信息");
                                dialog->resize(1100,200);
                                QTableWidget *table_tem=new QTableWidget(1,8,dialog);
                                table_tem->setEditTriggers(QAbstractItemView::NoEditTriggers);
                                table_tem->setMaximumSize(1100,200);
                                table_tem->setMinimumSize(1100,200);
                                QStringList header;
                                header<<"传感器编号"<<"名称"<<"规格"<<"型号"<<"生产厂家"<<"生产日期"<<"传感器类型"<<"单位";
                                table_tem->setHorizontalHeaderLabels(header);
                                dialog->show();

                                int j=0;
                                table_tem->setItem(0,j++,new QTableWidgetItem(Tem.get_identifier()));
                                table_tem->setItem(0,j++,new QTableWidgetItem(Tem.get_name()));
                                table_tem->setItem(0,j++,new QTableWidgetItem(Tem.get_specification()));
                                table_tem->setItem(0,j++,new QTableWidgetItem(Tem.get_version()));
                                table_tem->setItem(0,j++,new QTableWidgetItem(Tem.get_manufacturer()));
                                table_tem->setItem(0,j++,new QTableWidgetItem(Tem.get_produceDate()));
                                table_tem->setItem(0,j++,new QTableWidgetItem(QString::number(Tem.get_type())));
                                table_tem->setItem(0,j++,new QTableWidgetItem(Tem.get_unit()));

                                dialog->exec();
                            }
                        }
                );
           }
    );
}
void user::back_send()
{
    emit back_signal_user();
}
