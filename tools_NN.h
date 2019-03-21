//
// Created by nagi on 19-3-10.
//

#ifndef KAWORU_TOOLSFORCNN_H
#define KAWORU_TOOLSFORCNN_H
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <map>
#include "tools_general.h"
#include "tools_file.h"
using std::string;
using std::vector;
using std::map;

typedef map<string,int> MODE_MAP;
inline int ModeCode(string mode);
class ConvolutionCore;

MODE_MAP mode_map;
void mapIni(MODE_MAP &m) {
    m.insert(MODE_MAP::value_type("max",1));
    m.insert(MODE_MAP::value_type("average",2));
    m.insert(MODE_MAP::value_type("relu",3));
    m.insert(MODE_MAP::value_type("sigmoid",4));
}

class Nerv
{
private:
    vector<double> w;
public:
    vector<double> self;
    Nerv()= default;
    Nerv activationFunction(string mode);
};
Nerv Nerv::activationFunction(string mode)
{
    int modeCode = ModeCode(mode);
    switch (modeCode) {
        case 3:
            for(auto &a:self)
                a >= 0 ? a - a : a = 0;
            break;
        case 4:
            for(auto &a:self)
                a=1/(1+exp(-a));
            break;
    }
}

class ConvolutionNerv
{
private:
    int width,hight;
    void expand(int w,int h){self.expand(w,h);width=self.w();hight=self.h();}
    void expand(int w,int h,double n){self.expand(w,h,n);width=self.w();hight=self.h();}
public:
    Matrix beforePooling;
    Matrix self;
    friend ConvolutionNerv convoluting(ConvolutionNerv ori,ConvolutionCore core,int step);
	ConvolutionNerv()= default;
    ConvolutionNerv(int wd,int ht):width(wd),hight(ht){Matrix temp(wd,ht);self=temp;}
	inline int w(){return width;}
	inline int h(){return hight;}
    inline Matrix upsample(){return beforePooling;}
	inline int size(){return self.size();}
	ConvolutionNerv pooling(int d,string mode);
	ConvolutionNerv relu();
    vector<double>& operator[] (int i);
};
ConvolutionNerv ConvolutionNerv::pooling(int d,string mode)
{
	ConvolutionNerv return_temp=*this;
	int modeCode;
	modeCode=ModeCode(mode);
	if((return_temp.width)%d!=0||(return_temp.hight)%d!=0)
		expand(d-(return_temp.width)%d,d-(return_temp.hight)%d);
	ConvolutionNerv temp((return_temp.width)/d,(return_temp.hight)/d);
	switch(modeCode)
	{
		case 1:
			for(int ix=0,kx=0;ix<=return_temp.width-d;ix+=d,kx++)
			{
				for(int iy=0,ky=0;iy<=return_temp.hight-d;iy+=d,ky++)
				{
					int max = 0;
					int max_x=0,max_y=0;
					for (int x = 0; x < d; x++)
					{
						for (int y = 0; y < d; y++)
						{
							if (return_temp.self[ix + x][iy + y] > max)
							{
								max = return_temp.self[ix + x][iy + y];
								max_x=ix+x;
								max_y=iy+y;
							}
						}
					}
					temp.self[kx][ky]=max;
					return_temp.beforePooling[max_x][max_y]=max;
				}
			}
			break;
		case 2:
			for(int ix=0,kx=0;ix<=return_temp.width-d;ix+=d,kx++)
			{
				for(int iy=0,ky=0;iy<=return_temp.hight-d;iy+=d,ky++)
				{
					int sum;
					for (int x = 0; x < d; x++)
					{
						for (int y = 0; y < d; y++)
						{
								sum += return_temp.self[ix + x][iy + y];
						}
					}
					temp.self[kx][ky]=sum/(d*d);
					for (int x = 0; x < d; x++)
					{
						for (int y = 0; y < d; y++)
						{
							return_temp.beforePooling[ix + x][iy + y]=temp.self[kx][ky];
						}
					}
				}
			}
			break;

	}
	return return_temp;
}
ConvolutionNerv ConvolutionNerv::relu()
{
	ConvolutionNerv temp=*this;
	for(auto &v:temp.self)
	{
		for(auto &a:v)
		{
			if(a<0)
				a=0;
		}
	}
}
vector<double>& ConvolutionNerv::operator[] (int i)
{
    if(i<self.size())
        return self[i];
    else
    {
        std::cout<<"矩阵范围溢出"<<std::endl;
        return self[0];
    }
}

class ConvolutionCore:public ConvolutionNerv
{
private:
	int dimension;
	double bias;
public:
	ConvolutionCore()= default;
	ConvolutionCore(int d):dimension(d){Matrix temp(d,d);self=temp;}
	inline int d(){return dimension;}
	inline double b(){return bias;}
	void coreInitialize(int absoluteRage);
};
void ConvolutionCore::coreInitialize(int absoluteRage)
{
    bias=rdm(1);
	for(auto &v:self)
	{
		for(auto &a:v)
		{
			a=rdm(2*absoluteRage)-absoluteRage;
		}
	}
}

ConvolutionNerv convoluting(ConvolutionNerv ori,ConvolutionCore core,int step)
{
    if((ori.h()-core.d()%step)!=0||(ori.w()-core.d()%step)!=0)
        ori.expand(step-(ori.h()-core.d()%step),step-(ori.w()-core.d()%step));
    ConvolutionNerv result((ori.w()-core.d())/step+1,(ori.h()-core.d())/step+1);
	for(int ix=0,kx=0;ix<=ori.w()-core.d();ix+=step,kx++)
	{
		for(int iy=0,ky=0;iy<=ori.h()-core.d();iy+=step,ky++)
		{
			double sum=0;
			for (int x = 0; x < core.d(); x++)
			{
				for (int y = 0; y < core.d(); y++)
				{
					sum+=ori[ix + x][iy + y]*core[x][y]+core.b();
				}
			}
			result[kx][ky]=sum;
		}
	}
	return result;
}
double fullConnection(vector<double> m,ConvolutionNerv ori)
{
    int i=0;
    double sum=0;
    for(auto v:ori.self)
    {
        for(auto a:v)
        {
            sum+=m[i]*a;
            i++;
        }
    }
    return sum;
}

vector<double> softMax(vector<double> ori)
{
    double sum=0;
    vector<double> temp=ori;
    for(auto &a:temp)
    {
        a = exp(a);
        sum+=a;
    }
    for(auto &a:temp)
    {
       a=a/sum;
    }
    return temp;
}

inline double loss(double thr,double rslt,string type)
{
    if(type=="quadratic cost"&&type=="QC")
    	return (thr-rslt)*(thr-rslt)/2;
    else if(type=="cross entropy"&&type=="CE")
    	return -(thr*log(rslt)+(1-thr)*log(1-rslt));
}

inline int ModeCode(string mode)
{
    mapIni(mode_map);
    MODE_MAP::iterator iter;
    iter=mode_map.find(mode);
    if(iter != mode_map.end())
        return iter->second;
    else
        return 0;
}
#endif //KAWORU_TOOLSFORCNN_H
