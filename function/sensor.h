#ifndef SENSOR_H
#define SENSOR_H

#include <QString>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QMenu>
#include <QDialog>
#include <QHeaderView>

//传感器基类
class sensor
{
protected:
    QString identifier;             //传感器编号
    QString name;                  //名称
    QString specification;       //规格
    QString version;                //型号
    QString manufacturer;      //生产厂家
    QString produceDate;       //生产日期
    int  type;                            //传感器类型
public:
    sensor(){}

    sensor(const sensor &x)
    {
        identifier=x.identifier;
        name=x.name;
        specification=x.specification;
        version=x.version;
        manufacturer=x.manufacturer;
        produceDate=x.produceDate;
        type=x.type;
    }
};

//应变传感器类
class strainSensor:public sensor
{
private:
    double standardwavelength;
    friend class strainSet;
    friend class strain_cp;
public:
    strainSensor(){}

    strainSensor(const strainSensor &x):sensor(x)
    {
        standardwavelength=x.standardwavelength;
    }

    void set_identifier(QString x)
    {
        identifier = x;
    }
    void set_name(QString x)
    {
        name = x;
    }
    void set_specification(QString x)
    {
        specification = x;
    }
    void set_version(QString x)
    {
        version = x;
    }
    void set_manufacturer(QString x)
    {
        manufacturer = x;
    }
    void set_produceDate(QString x)
    {
        produceDate = x;
    }
    void set_type(int x)
    {
        type = x;
    }
    QString get_identifier()
    {
        return identifier ;
    }
    QString get_name()
    {
        return name ;
    }
    QString get_specification()
    {
        return specification;
    }
    QString get_version()
    {
        return version;
    }
    QString get_manufacturer()
    {
        return manufacturer;
    }
    QString get_produceDate()
    {
        return produceDate;
    }
    int get_type()
    {
        return type;
    }


    void set_standardwavelength(double x)
    {
        standardwavelength = x;
    }
    double get_standardwavelength()
    {
        return standardwavelength;
    }
};

//桡度传感器类
class deflectionSensor:public sensor
{
private:
    QString unit;
    friend class deflectionSet;
public:
    deflectionSensor(){}

    deflectionSensor(const deflectionSensor &x):sensor(x)
    {
        unit=x.unit;
    }

    void set_identifier(QString x)
    {
        identifier = x;
    }
    void set_name(QString x)
    {
        name = x;
    }
    void set_specification(QString x)
    {
        specification = x;
    }
    void set_version(QString x)
    {
        version = x;
    }
    void set_manufacturer(QString x)
    {
        manufacturer = x;
    }
    void set_produceDate(QString x)
    {
        produceDate = x;
    }
    void set_type(int x)
    {
        type = x;
    }
    QString get_identifier()
    {
        return identifier;
    }
    QString get_name()
    {
        return name;
    }
    QString get_specification()
    {
        return specification;
    }
    QString get_version()
    {
        return version;
    }
    QString get_manufacturer()
    {
        return manufacturer;
    }
    QString get_produceDate()
    {
        return produceDate;
    }
    int get_type()
    {
        return type;
    }

    void set_unit(QString x)
    {
        unit = x;
    }
    QString get_unit()
    {
        return unit;
    }
};

//位移传感器类
class displacementSensor :public sensor
{
private:
    double upper_limit;
    double lower_limit;
    double init;
    friend class displacementSet;
public:
    displacementSensor(){}

    displacementSensor(const displacementSensor &x):sensor(x)
    {
        upper_limit=x.upper_limit;
        lower_limit=x.lower_limit;
        init=x.init;
    }

    void set_identifier(QString x)
    {
        identifier = x;
    }
    void set_name(QString x)
    {
        name = x;
    }
    void set_specification(QString x)
    {
        specification = x;
    }
    void set_version(QString x)
    {
        version = x;
    }
    void set_manufacturer(QString x)
    {
        manufacturer = x;
    }
    void set_produceDate(QString x)
    {
        produceDate = x;
    }
    void set_type(int x)
    {
        type = x;
    }
    QString get_identifier()
    {
        return identifier;
    }
    QString get_name()
    {
        return name;
    }
    QString get_specification()
    {
        return specification;
    }
    QString get_version()
    {
        return version;
    }
    QString get_manufacturer()
    {
        return manufacturer;
    }
    QString get_produceDate()
    {
        return produceDate;
    }
    int get_type()
    {
        return type;
    }

    void set_upper_limit(double x)
    {
        upper_limit = x;
    }
    void set_lower_limit(double x)
    {
        lower_limit = x;
    }
    void set_init(double x)
    {
         init= x;
    }
    double get_upper_limit()
    {
        return upper_limit;
    }
    double get_lower_limit()
    {
        return lower_limit ;
    }
    double get_init()
    {
        return init;
    }

};

//---------------------------------------------------------------------------------------------------------------------------------------------------

class strainSet
{
public:
    strainSensor strain_s[20];
    int num;
    int delet(QString x)
    {
            for(int i=0;i<num;i++)
            {
                if(x==strain_s[i].identifier)
                {
                    for(int j=i;j<num-1;j++)
                    {
                        strain_s[j]=strain_s[j+1];
                    }
                    num--;
                    return 1;
                }
            }

            return 0;
    }
    int search(QString x)
    {
        for(int i=0;i<num;i++)
        {
            if(x==strain_s[i].identifier)
            {
                return i;                               //搜索成功，返回在数组中的位次
            }
        }
        return -1;                                  //搜索失败
    }
    void put_in_file()
    {
        QFile file("../files/strain_sensor_File.txt");
        bool isok=file.open(QIODevice::Text | QIODevice::WriteOnly);

        if(isok==true)
        {
            QTextStream stream(&file);
            stream.setCodec("UTF-8");
            stream.setRealNumberNotation(QTextStream::FixedNotation);
            stream.setRealNumberPrecision(5);
            for(int i=0;i<num;i++)
            {
                stream<<strain_s[i].identifier<<" "<<strain_s[i].name<<" "
                           <<strain_s[i].specification<<" "<<strain_s[i].version<<" "
                           <<strain_s[i].manufacturer<<" "<<strain_s[i].produceDate<<" "
                           <<strain_s[i].type<<" "<<strain_s[i].standardwavelength;
                if(i!=num-1)
                    stream<<endl;
            }
        }

        file.close();
    }
    void read_from_file()
    {
        QFile file("../files/strain_sensor_File.txt");
        bool isok=file.open(QIODevice::Text | QIODevice::ReadOnly);

        if(isok==true)
        {
            QTextStream stream(&file);
            stream.setCodec("UTF-8");
            int i=0;
            QString tem;
            while(false==stream.atEnd())
            {
                stream >>strain_s[i].identifier>>strain_s[i].name
                            >>strain_s[i].specification>>strain_s[i].version
                            >>strain_s[i].manufacturer>>strain_s[i].produceDate
                            >>strain_s[i].type>>tem;
                strain_s[i].standardwavelength=tem.toDouble();
                i++;
            }
            num=i;
            file.close();
        }

    }
    strainSensor get_sensor(int x)
    {
        return strain_s[x];
    }
};

class deflectionSet//挠度传感器管理类
{
public:
    deflectionSensor def_s[20];
    int num;
    int delet(QString x)
    {
            for(int i=0;i<num;i++)
            {
                if(x==def_s[i].identifier)
                {
                    for(int j=i;j<num-1;j++)
                    {
                        def_s[j]=def_s[j+1];
                    }
                    num--;
                    return 1;
                }
            }

            return 0;
    }
    int search(QString x)
    {
        for(int i=0;i<num;i++)
        {
            if(x==def_s[i].identifier)
            {
                return i;                               //搜索成功，返回在数组中的位次
            }
        }
        return -1;                                  //搜索失败
    }
    void put_in_file()
    {
        QFile file("../files/deflection_sensor_File.txt");
        bool isok=file.open(QIODevice::Text | QIODevice::WriteOnly);

        if(isok==true)
        {
            QTextStream stream(&file);
            stream.setCodec("UTF-8");
            for(int i=0;i<num;i++)
            {
                stream<<def_s[i].identifier<<" "<<def_s[i].name<<" "
                           <<def_s[i].specification<<" "<<def_s[i].version<<" "
                           <<def_s[i].manufacturer<<" "<<def_s[i].produceDate<<" "
                           <<def_s[i].type<<" "<<def_s[i].unit;
                if(i!=num-1)
                    stream<<endl;
            }
        }

        file.close();
    }
    void read_from_file()
    {
        QFile file("../files/deflection_sensor_File.txt");
        bool isok=file.open(QIODevice::Text | QIODevice::ReadOnly);

        if(isok==true)
        {
            QTextStream stream(&file);
            stream.setCodec("UTF-8");
            int i=0;
            while(false==stream.atEnd())
            {
                stream >>def_s[i].identifier>>def_s[i].name
                            >>def_s[i].specification>>def_s[i].version
                            >>def_s[i].manufacturer>>def_s[i].produceDate>>def_s[i].type
                            >>def_s[i].unit;
                i++;
            }
            num=i;
            file.close();
        }

    }
    deflectionSensor get_sensor(int x)
    {
        return def_s[x];
    }
};

class displacementSet
{
public:
    displacementSensor dis_s[10];
    int num;
    int search(QString x)
    {
        for(int i=0;i<num;i++)
        {
            if(x==dis_s[i].identifier)
            {
                return i;                               //搜索成功，返回在数组中的位次
            }
        }
        return -1;                                  //搜索失败
    }
    int delet(QString x)
    {
        for(int i=0;i<num;i++)
        {
            if(x==dis_s[i].identifier)
            {
                for(int j=i;j<num-1;j++)
                {
                    dis_s[j]=dis_s[j+1];
                }
                num--;
                return 1;
            }
        }

    return 0;
}
    void put_in_file()
    {
        QFile file("../files/displacement_sensor_File.txt");
        bool isok=file.open(QIODevice::Text | QIODevice::WriteOnly);

        if(isok==true)
        {
            QTextStream stream(&file);
            stream.setCodec("UTF-8");
            for(int i=0;i<num;i++)
            {
                stream<<dis_s[i].identifier<<" "<<dis_s[i].name<<" "
                           <<dis_s[i].specification<<" "<<dis_s[i].version<<" "
                           <<dis_s[i].manufacturer<<" "<<dis_s[i].produceDate<<" "
                           <<dis_s[i].type<<" "<<dis_s[i].init<<" "
                           <<dis_s[i].upper_limit<<" "<<dis_s[i].lower_limit;
                if(i!=num-1)
                    stream<<endl;
            }
        }

        file.close();
   }
    void read_from_file()
    {
        QFile file("../files/displacement_sensor_File.txt");
        bool isok=file.open(QIODevice::Text | QIODevice::ReadOnly);

        if(isok==true)
        {
            QTextStream stream(&file);
            stream.setCodec("UTF-8");
            int i=0;
            while(false==stream.atEnd())
            {
                stream >>dis_s[i].identifier>>dis_s[i].name
                            >>dis_s[i].specification>>dis_s[i].version
                            >>dis_s[i].manufacturer>>dis_s[i].produceDate
                            >>dis_s[i].type>>dis_s[i].init>>dis_s[i].upper_limit
                            >>dis_s[i].lower_limit;
                i++;
            }
            num=i;
            file.close();
        }
    }
    displacementSensor get_sensor(int x)
    {
        return dis_s[x];
    }
};

#endif // SENSOR_H
