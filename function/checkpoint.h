#ifndef CHECKPOINT_H
#define CHECKPOINT_H
//！！！！对于原始数据的读入需要重新写函数
#include <QString>
#include "sensor.h"
#include "data.h"
#include "cpset.h"

//测点基类
class checkpoint
{
protected:
        QString  identifier;    //测点编号
        QString name;           //测点名字
        QString location;      //测点位置
        int type;                   //测点类型
public:
        checkpoint(){}
        //拷贝构造函数，管理类要用
        checkpoint(const checkpoint &x)
        {
            identifier=x.identifier;
            name=x.name;
            location=x.location;
            type=x.type;
        }

};

//应变测点类
class strain_cp :public checkpoint {
    private:
        strainSensor Asensor;           //传感器
        strainDataSet pri_data;            //原始数据
        StrainDataSet ans_data;            //结果数据
        friend class strain_cp_Set;
    public:
        strain_cp(){}

        strain_cp(const strain_cp &x):checkpoint(x)
        {
            Asensor=x.Asensor;
            pri_data=x.pri_data;
            ans_data=x.ans_data;
        }

        void set_Asensor(strainSensor x)
        {
            Asensor=x;
        }
        void set_pri_data(strainDataSet x)
        {
            pri_data=x;
        }

        void set_ans_data(StrainDataSet x)
        {
            ans_data=x;
        }
        strainSensor get_Asensor()
        {
            return Asensor;
        }
        strainDataSet get_pri_data()
        {
            return pri_data;
        }
        StrainDataSet get_ans_data()
        {
             return ans_data;
        }

        void set_identifier(QString x)
        {
            identifier=x;
        }
        void set_name(QString x)
        {
            name=x;
        }
        void set_location(QString x)
        {
            location=x;
        }
        void set_type(int x)
        {
            type=x;
        }
        QString get_identifier()
        {
            return identifier;
        }
        QString get_name()
        {
            return name;
        }
        QString get_location()
        {
            return location;
        }
        int get_type()
        {
            return type;
        }
        void calculate()
        {
            for(int i=0;i<pri_data.num;i++)
            {
                ans_data.Strain_data_s[i].date=pri_data.strain_data_s[i].date;
                ans_data.Strain_data_s[i].time=pri_data.strain_data_s[i].time;
                //1000*（实际波长-该传感器的中心波长）+ 8 * 0.5
                ans_data.Strain_data_s[i].ans=1000*(pri_data.strain_data_s[i].wavelength-Asensor.standardwavelength)+8*0.5;
            }
            ans_data.num=pri_data.num;
        }
};

//桡度测点类
class deflection_cp :public checkpoint {
    private:
        deflectionSensor Asensor;
        deflectionDataSet pri_data;
        DeflectionDataSet ans_data;
        friend class deflection_cp_Set;
    public:
        deflection_cp(){}

        deflection_cp(const deflection_cp &x):checkpoint(x)
        {
            Asensor=x.Asensor;
            pri_data=x.pri_data;
            ans_data=x.ans_data;
        }
        void set_name(QString x)
        {
            name=x;
        }
        QString get_name()
        {
            return name;
        }

        void set_Asensor(deflectionSensor x)
        {
            Asensor=x;
        }
        void set_pri_data(deflectionDataSet x)
        {
            pri_data=x;
        }
        void set_ans_data(DeflectionDataSet x)
        {
            ans_data=x;
        }
        deflectionSensor get_Asensor()
        {
            return Asensor;
        }
        deflectionDataSet get_pri_data()
        {
            return pri_data;
        }
        DeflectionDataSet get_ans_data()
        {
             return ans_data;
        }

        void set_identifier(QString x)
        {
            identifier=x;
        }
        void set_location(QString x)
        {
            location=x;
        }
        void set_type(int x)
        {
            type=x;
        }
        QString get_identifier()
        {
            return identifier;
        }
        QString get_location()
        {
            return location;
        }
        int get_type()
        {
            return type;
        }
        void calculate(deflectionDataSet init_pri_data)//计算挠度
        {
            for(int i=0;i<pri_data.num;i++)
            {
                    ans_data.Deflecton_data_s[i].date=pri_data.deflecton_data_s[i].date;
                    ans_data.Deflecton_data_s[i].time=pri_data.deflecton_data_s[i].time;
                    ans_data.Deflecton_data_s[i].code=pri_data.deflecton_data_s[i].code;
                    ans_data.Deflecton_data_s[i].unit=pri_data.deflecton_data_s[i].unit;

                    //挠度=检测值-700737的检测值
                    ans_data.Deflecton_data_s[i].def_ans=pri_data.deflecton_data_s[i].def_num-init_pri_data.deflecton_data_s[i].def_num;
            }
            ans_data.num=pri_data.num;
        }

};


//位移测点类
class displacement_cp :public checkpoint {
    private:
        displacementSensor Asensor;
        displaceDataSet pri_data;
        DisplaceDataSet ans_data;
        friend class displacement_cp_Set;
    public:
        displacement_cp(){}
        //拷贝构造函数，管理类要用
        displacement_cp(const displacement_cp &x):checkpoint(x)
        {
            Asensor=x.Asensor;
            pri_data=x.pri_data;
            ans_data=x.ans_data;
        }

        void set_Asensor(displacementSensor x)
        {
            Asensor=x;
        }
        void set_pri_data(displaceDataSet x)
        {
            pri_data=x;
        }
        void set_ans_data(DisplaceDataSet x)
        {
            ans_data=x;
        }
        displacementSensor get_Asensor()
        {
            return Asensor;
        }
        displaceDataSet get_pri_data()
        {
            return pri_data;
        }
        DisplaceDataSet get_ans_data()
        {
             return ans_data;
        }

        void set_identifier(QString x)
        {
            identifier=x;
        }
        void set_name(QString x)
        {
            name=x;
        }

        void set_location(QString x)
        {
            location=x;
        }
        void set_type(int x)
        {
            type=x;
        }
        QString get_identifier()
        {
            return identifier;
        }
        QString get_name()
        {
            return name;
        }
        QString get_location()
        {
            return location;
        }
        int get_type()
        {
            return type;
        }
        void calculate()
        {

            for(int i=0;i<pri_data.num;i++)
            {
                ans_data.Dis_data_s[i].date=pri_data.dis_data_s[i].date;
                ans_data.Dis_data_s[i].time=pri_data.dis_data_s[i].time;
                ans_data.Dis_data_s[i].code=pri_data.dis_data_s[i].code;
                ans_data.Dis_data_s[i].unit=pri_data.dis_data_s[i].unit;
                ans_data.Dis_data_s[i].dis_ans=pri_data.dis_data_s[i].dis_num-Asensor.get_init();

            }
            ans_data.num=pri_data.num;
        }
};

#endif // CHECKPOINT_H
