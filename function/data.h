#ifndef DATA_H
#define DATA_H
#include <QString>

//应变原始数据
class strainData
{
public:
    QString date;
    QString time;
    double wavelength;
};
class strainDataSet
{
public:
    strainData strain_data_s[50];
    int num;
};

//应变计算数据
class StrainData
{
public:
    QString date;
    QString time;
    double ans;
};
class StrainDataSet
{
public:
    StrainData Strain_data_s[50];
    int num;
};

//桡度原始数据
class deflectionData
{
public:
    QString  date;
    QString  time;
    int code;//传感器编号
    double def_num;//检测值

    QString unit;
};
class deflectionDataSet
{
public:
    deflectionData deflecton_data_s[50];
    int num;
};

//桡度计算数据
class DeflectionData
{
public:
    QString  date;
    QString  time;
    int code;//传感器编号
    double def_ans;//结果值
    QString unit;
};
class DeflectionDataSet
{
public:
    DeflectionData Deflecton_data_s[50];
    int num;
};

//位移原始数据
class displaceData
{
public:
    QString date;
    QString time;
    int code;
    double dis_num;
    QString unit;
};
class displaceDataSet
{
public:
    displaceData dis_data_s[50];
    int num;
};

//位移计算数据
class DisplaceData
{
public:
    QString date;
    QString time;
    int code;
    double dis_ans;
    QString unit;
};
class DisplaceDataSet
{
public:
    DisplaceData Dis_data_s[50];
    int num;
};
#endif // DATA_H
