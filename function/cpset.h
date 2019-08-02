#ifndef CPSET_H
#define CPSET_H

#include "checkpoint.h"
#include "data.h"
#include <QFile>
#include <QTextStream>
#include <sensor.h>
#include <QDebug>

//应变测点管理类
class strain_cp_Set
{
private:
public:
    strain_cp  strain_cps[20];
    int num;
    int delet(QString x)
    {
        for(int i=0;i<num;i++)
        {
            if(x==strain_cps[i].identifier)
            {
                for(int j=i;j<num-1;j++)
                {
                    strain_cps[j]=strain_cps[j+1];
                }
                num--;
                return 1;//删除成功
            }
        }
        return 0;//删除失败
    }

    int search(QString x)
    {
        for(int i=0;i<num;i++)
        {
            if(x==strain_cps[i].identifier)
            {
                return i;                               //搜索成功，返回在数组中的位次
            }
        }
        return -1;                                  //搜索失败
    }

    void put_in_file()
    {
        QFile file("../files/strain_cp_File.txt");
        bool isok=file.open(QIODevice::Text |QIODevice::WriteOnly);
        if(isok==true)
        {
            QTextStream stream(&file);
            stream.setCodec("UTF-8");
            for(int i=0;i<num;i++)
            {
                stream <<strain_cps[i].get_identifier()<<" "
                            <<strain_cps[i].get_name()<<" "
                            <<strain_cps[i].get_location()<<" "
                            <<strain_cps[i].get_type()<<" "
                           <<strain_cps[i].get_Asensor().get_identifier();
                if(i!=num-1)
                    stream<<endl;
           }
        }
        file.close();
    }

    void read_from_file()
    {
        QFile file("../files/strain_cp_File.txt");
        bool isok = file.open(QIODevice::Text | QIODevice::ReadOnly);

        if(isok==true)
        {
            QTextStream stream(&file);
            stream.setCodec("UTF-8");

            int i=0;
            while(stream.atEnd()==false)
            {
                    stream >> strain_cps[i].identifier;
                    stream >> strain_cps[i].name;
                    stream >> strain_cps[i].location;
                    stream >> strain_cps[i].type;
                    QString tem;
                    stream >>tem;
                    //这里要根据tem确定传感器
                    strainSet Tem;
                    Tem.read_from_file();
                    strain_cps[i].set_Asensor(Tem.get_sensor(Tem.search(tem)));
                    i++;
             }
             num=i;
        }

        //获取测点组中的ans_data成员，但不必再获得pri_data成员，除了初始化需要外不再需要pri_data
        read_ans_data();

        file.close();

    }

    void read_pri_data()
    {
        QFile file("../data/strain_data.txt");
        bool isok = file.open(QIODevice::Text | QIODevice::ReadOnly);

        if(isok==true)
        {
            QTextStream stream(&file);
            stream.setCodec("UTF-8");

            QString date_tem;
            QString time_tem;
            QString tem;

            int i=0;
            while(stream.atEnd()==false)
            {
                stream >>date_tem;
                //qDebug()<<date_tem;
                stream >>time_tem;
                //qDebug()<<time_tem;
                for(int j=0;j<10;j++)
                {
                    stream >>tem;
                    //qDebug()<<tem;
                    strain_cps[j].pri_data.strain_data_s[i].date=date_tem;
                    strain_cps[j].pri_data.strain_data_s[i].time=time_tem;
                    strain_cps[j].pri_data.strain_data_s[i].wavelength=tem.toDouble();
                    //qDebug()<<fixed<<qSetRealNumberPrecision(5) <<strain_cps[j].pri_data.strain_data_s[i].wavelength;
                    //上一行代码说明了QString转化为double类型是不会丢失精度的
                }
                i++;
             }
            for(int j=0;j<10;j++)
            {
                strain_cps[j].pri_data.num=i;
            }
         }

        file.close();
     }

    void read_ans_data()
    {
        QFile file("../data/Strain_ans_data.txt");
        bool isok = file.open(QIODevice::Text | QIODevice::ReadOnly);
        if(true==isok)
        {
            QTextStream stream(&file);
            stream.setCodec("UTF-8");

            QString tem;
            int i=0;
            while(stream.atEnd()==false)
            {
                stream >> strain_cps[i].ans_data.num;
                for(int j=0;j<strain_cps[i].ans_data.num;j++)
                {
                    stream >>strain_cps[i].ans_data.Strain_data_s[j].date;
                    stream >>strain_cps[i].ans_data.Strain_data_s[j].time;
                    stream >>tem;
                    strain_cps[i].ans_data.Strain_data_s[j].ans=tem.toDouble();
                    //qDebug()<<fixed<<qSetRealNumberPrecision(5) <<strain_cps[i].ans_data.Strain_data_s[j].ans;
                }
                i++;
            }
        }
        file.close();
    }

    void write_ans_data()
    {
        QFile file("../data/Strain_ans_data.txt");
        bool isok = file.open(QIODevice::Text | QIODevice::WriteOnly);
        if(true==isok)
        {
            QTextStream stream(&file);
            stream.setCodec("UTF-8");
            stream.setRealNumberNotation(QTextStream::FixedNotation);
            stream.setRealNumberPrecision(5);

            for(int i=0;i<num;i++)
            {
                    stream <<strain_cps[i].ans_data.num<<endl;
                    for(int j=0;j<strain_cps[i].ans_data.num;j++)
                    {
                        stream <<strain_cps[i].ans_data.Strain_data_s[j].date<<" ";
                        stream <<strain_cps[i].ans_data.Strain_data_s[j].time<<" ";
                        stream <<strain_cps[i].ans_data.Strain_data_s[j].ans;

                        if(i!=num-1 || j!=strain_cps[i].ans_data.num-1)
                            stream<<endl;
                    }
            }
        }
        file.close();
    }
};

//桡度测点管理类
class deflection_cp_Set
{
public:
    deflection_cp deflection_cps[20];
    int num;
    int delet(QString x)
    {
        for(int i=0;i<num;i++)
        {
            if(x==deflection_cps[i].identifier)
            {
                for(int j=i;j<num-1;j++)
                {
                    deflection_cps[j]=deflection_cps[j+1];
                }
                num--;
                return 1;//删除成功
            }
        }
        return 0;//删除失败
    }

    int search(QString x)
    {
        for(int i=0;i<num;i++)
        {
            if(x==deflection_cps[i].identifier)
            {
                return i;                               //搜索成功，返回在数组中的位次
            }
        }
        return -1;                                  //搜索失败
    }

    void put_in_file()
    {
        QFile file("../files/deflection_cp_File.txt");
        bool isok=file.open(QIODevice::Text |QIODevice::WriteOnly);
        if(isok==true)
        {
            QTextStream stream(&file);
            stream.setCodec("UTF-8");
            for(int i=0;i<num;i++)
            {
                stream <<deflection_cps[i].get_identifier()<<" "
                         <<deflection_cps[i].get_name()<<" "
                            <<deflection_cps[i].get_location()<<" "
                            <<deflection_cps[i].get_type()<<" "

                           <<deflection_cps[i].get_Asensor().get_identifier();
                if(i!=num-1)
                    stream<<endl;
           }
        }
        file.close();
    }

    void read_from_file()
    {
        QFile file("../files/deflection_cp_File.txt");
        bool isok = file.open(QIODevice::Text | QIODevice::ReadOnly);

        if(isok==true)
        {
            QTextStream stream(&file);
            stream.setCodec("UTF-8");

            int i=0;
            while(stream.atEnd()==false)
            {
                    stream >> deflection_cps[i].identifier;
                    stream >> deflection_cps[i].name;
                    stream >> deflection_cps[i].location;
                    stream >> deflection_cps[i].type;

                    QString tem;
                    stream >>tem;
                    deflectionSet Tem;
                    Tem.read_from_file();
                    deflection_cps[i].set_Asensor(Tem.get_sensor(Tem.search(tem)));
                    i++;
             }
             num=i;
        }
        read_ans_data();

        file.close();

    }

    void read_pri_data()
        {
            QFile file("../data/deflection_data.txt");
            bool isok = file.open(QIODevice::Text | QIODevice::ReadOnly);

            if(isok==true)
            {
                QTextStream stream(&file);
                stream.setCodec("UTF-8");

                QString date_tem;
                QString time_tem;
                QString code_tem;
                QString def_num_tem;
                QString unit_tem;

                int i=0;
                while(stream.atEnd()==false)
                {
                    for(int j=0;j<14;j++)
                    {
                        stream >>date_tem;
                        stream >>time_tem;
                        stream >>code_tem;
                        stream >>def_num_tem;
                        stream >>unit_tem;
                        deflection_cps[j].pri_data.deflecton_data_s[i].date=date_tem;
                        deflection_cps[j].pri_data.deflecton_data_s[i].time=time_tem;
                        deflection_cps[j].pri_data.deflecton_data_s[i].code=code_tem.toInt();
                        deflection_cps[j].pri_data.deflecton_data_s[i].def_num=def_num_tem.toDouble();
                        deflection_cps[j].pri_data.deflecton_data_s[i].unit=unit_tem;
                    }
                    i++;
                 }
                for(int j=0;j<14;j++)
                {
                    deflection_cps[j].pri_data.num=i;
                }
             }
             file.close();
         }

    void read_ans_data()
    {
            QFile file("../data/Deflection_ans_data.txt");
            bool isok = file.open(QIODevice::Text | QIODevice::ReadOnly);
            if(true==isok)
            {
                QTextStream stream(&file);
                stream.setCodec("UTF-8");
                QString code_tem;
                QString def_ans_tem;
                int i=0;
                while(stream.atEnd()==false)
                {
                    stream >> deflection_cps[i].ans_data.num;
                    for(int j=0;j<deflection_cps[i].ans_data.num;j++)
                    {
                        stream >>deflection_cps[i].ans_data.Deflecton_data_s[j].date;
                        stream >>deflection_cps[i].ans_data.Deflecton_data_s[j].time;
                        stream >>code_tem;
                        deflection_cps[i].ans_data.Deflecton_data_s[j].code=code_tem.toInt();
                        stream >>def_ans_tem;
                        deflection_cps[i].ans_data.Deflecton_data_s[j].def_ans=def_ans_tem.toDouble();
                        stream >>deflection_cps[i].ans_data.Deflecton_data_s[j].unit;
                    }
                    i++;
                }
            }
            file.close();
        }

    void write_ans_data()
    {
            QFile file("../data/Deflection_ans_data.txt");
            bool isok = file.open(QIODevice::Text | QIODevice::WriteOnly);
            if(true==isok)
            {
                QTextStream stream(&file);
                stream.setCodec("UTF-8");
                stream.setRealNumberNotation(QTextStream::FixedNotation);
                stream.setRealNumberPrecision(5);

                for(int i=0;i<num;i++)
                {
                        stream <<deflection_cps[i].ans_data.num<<endl;
                        for(int j=0;j<deflection_cps[i].ans_data.num;j++)
                        {
                            stream <<deflection_cps[i].ans_data.Deflecton_data_s[j].date<<" ";
                            stream <<deflection_cps[i].ans_data.Deflecton_data_s[j].time<<" ";
                            stream <<deflection_cps[i].ans_data.Deflecton_data_s[j].code<<" ";
                            stream <<deflection_cps[i].ans_data.Deflecton_data_s[j].def_ans<<" ";
                            stream <<deflection_cps[i].ans_data.Deflecton_data_s[j].unit;

                            if(i!=num-1 || j!=deflection_cps[i].ans_data.num-1)
                                stream<<endl;
                        }
                }
            }
            file.close();
        }
};

//位移测点管理类
class displacement_cp_Set
{
public:
    displacement_cp  displacement_cps[5];
    int num;
    int delet(QString x)
    {
        for(int i=0;i<num;i++)
        {
            if(x==displacement_cps[i].identifier)
            {
                for(int j=i;j<num-1;j++)
                {
                    displacement_cps[j]=displacement_cps[j+1];
                }
                num--;
                return 1;//删除成功
            }
        }
        return 0;//删除失败
    }

    int search(QString x)
    {
        for(int i=0;i<num;i++)
        {
            if(x==displacement_cps[i].identifier)
            {
                return i;                               //搜索成功，返回在数组中的位次
            }
        }
        return -1;                                  //搜索失败
    }

    void put_in_file()
    {
        QFile file("../files/displacement_cp_File.txt");
        bool isok=file.open(QIODevice::Text |QIODevice::WriteOnly);
        if(isok==true)
        {
            QTextStream stream(&file);
            stream.setCodec("UTF-8");
            for(int i=0;i<num;i++)
            {
                stream <<displacement_cps[i].get_identifier()<<" "
                           <<displacement_cps[i].get_name()<<" "
                           <<displacement_cps[i].get_location()<<" "
                           <<displacement_cps[i].get_type()<<" "
                           <<displacement_cps[i].get_Asensor().get_identifier();
                if(i!=num-1)
                    stream<<endl;
           }
        }
        file.close();
    }

    void read_from_file()
    {
        QFile file("../files/displacement_cp_File.txt");
        bool isok = file.open(QIODevice::Text | QIODevice::ReadOnly);

        if(isok==true)
        {
            QTextStream stream(&file);
            stream.setCodec("UTF-8");

            int i=0;
            while(stream.atEnd()==false)
            {
                    stream >> displacement_cps[i].identifier;
                    stream >> displacement_cps[i].name;
                    stream >> displacement_cps[i].location;
                    stream >> displacement_cps[i].type;
                    QString tem;
                    stream >>tem;
                    displacementSet Tem;
                    Tem.read_from_file();
                    displacement_cps[i].set_Asensor(Tem.get_sensor(Tem.search(tem)));
                    i++;
             }
             num=i;
        }
        read_ans_data();

        file.close();
    }

    void read_pri_data()
    {
        QFile file("../data/displacement_data.txt");
        bool isok = file.open(QIODevice::Text | QIODevice::ReadOnly);

        if(isok==true)
        {
            QTextStream stream(&file);
            stream.setCodec("UTF-8");

            QString date_tem;
            QString time_tem;
            QString code_tem;

            QString tem;
            QString unit_tem;
            int i=0;
            while(stream.atEnd()==false)
            {

                for(int j=0;j<num;j++)
                {
                    stream >>date_tem;
                    stream >>time_tem;
                    stream >> code_tem;
                    stream >>tem;
                    stream>>unit_tem;
                    displacement_cps[j].pri_data.dis_data_s[i].date=date_tem;
                    displacement_cps[j].pri_data.dis_data_s[i].time=time_tem;
                    displacement_cps[j].pri_data.dis_data_s[i].code=code_tem.toInt();
                    displacement_cps[j].pri_data.dis_data_s[i].dis_num=tem.toDouble();
                    displacement_cps[j].pri_data.dis_data_s[i].unit=unit_tem;
                }
                i++;
             }
            for(int j=0;j<num;j++)
            {
                displacement_cps[j].pri_data.num=i;
            }
         }
                file.close();
     }

    void read_ans_data()
    {
        QFile file("../data/Displacement_ans_data.txt");
        bool isok = file.open(QIODevice::Text | QIODevice::ReadOnly);
        if(true==isok)
        {
            QTextStream stream(&file);
            stream.setCodec("UTF-8");

            QString tem;
            int i=0;
            while(stream.atEnd()==false)
            {
                stream >> displacement_cps[i].ans_data.num;
                for(int j=0;j<displacement_cps[i].ans_data.num;j++)
                {
                    stream >>displacement_cps[i].ans_data.Dis_data_s[j].date;
                    stream >>displacement_cps[i].ans_data.Dis_data_s[j].time;
                    stream >>displacement_cps[i].ans_data.Dis_data_s[j].code;
                    stream >>tem;
                    displacement_cps[i].ans_data.Dis_data_s[j].dis_ans=tem.toDouble();
                    stream >>displacement_cps[i].ans_data.Dis_data_s[j].unit;
                }
                i++;
            }
        }
        file.close();
    }

    void write_ans_data()
    {
        QFile file("../data/Displacement_ans_data.txt");
        bool isok = file.open(QIODevice::Text | QIODevice::WriteOnly);
        if(true==isok)
        {
            QTextStream stream(&file);
            stream.setCodec("UTF-8");
            stream.setRealNumberNotation(QTextStream::FixedNotation);
            stream.setRealNumberPrecision(5);

            for(int i=0;i<num;i++)
            {
                    stream <<displacement_cps[i].ans_data.num<<endl;
                    for(int j=0;j<displacement_cps[i].ans_data.num;j++)
                    {
                        stream <<displacement_cps[i].ans_data.Dis_data_s[j].date<<" ";
                        stream <<displacement_cps[i].ans_data.Dis_data_s[j].time<<" ";
                        stream <<displacement_cps[i].ans_data.Dis_data_s[j].code<<" ";
                        stream <<displacement_cps[i].ans_data.Dis_data_s[j].dis_ans<<" ";
                        stream <<displacement_cps[i].ans_data.Dis_data_s[j].unit;

                        if(i!=num-1 || j!=displacement_cps[i].ans_data.num-1)
                            stream<<endl;
                    }
            }
        }
        file.close();
    }
};


#endif // CPSET_H
