//
// Created by nagi on 19-3-10.
//

#ifndef KAWORU_TOOLSFORCNN_H
#define KAWORU_TOOLSFORCNN_H
#include <iostream>
#include <string>
#include <vector>
#include "tools_general.h"
#include "tools_file.h"
using std::string;

class ConvolutionCore;

class Convolution
{
private:
	int width,hight;
	Matrix beforePooling;
	void expand(int w,int h){self.expand(w,h);width=self.w();hight=self.h();}
	void expand(int w,int h,double n){self.expand(w,h,n);width=self.w();hight=self.h();}
public:
    friend Convolution convoluting(Convolution ori,ConvolutionCore core,int step);
	Matrix self;
	Convolution()= default;
	Convolution(int wd,int ht):width(wd),hight(ht){Matrix temp(wd,ht);self=temp;}
	inline int w(){return width;}
	inline int h(){return hight;}
	inline int size(){return self.size();}
	Convolution pooling(int d,string mode);
	Convolution relu();
    vector<double>& operator[] (int i);
};
Convolution Convolution::pooling(int d,string mode)
{
	Convolution return_temp=*this;
	int modeCode;
	if(mode=="max")
		modeCode=0;
	else if(mode=="average")
		modeCode=1;
	if((return_temp.width)%d!=0||(return_temp.hight)%d!=0)
		expand(d-(return_temp.width)%d,d-(return_temp.hight)%d);
	Convolution temp((return_temp.width)/d,(return_temp.hight)/d);
	switch(modeCode)
	{
		case 0:
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
		case 1:
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
Convolution Convolution::relu()
{
	Convolution temp=*this;
	for(auto &v:temp.self)
	{
		for(auto &a:v)
		{
			if(a<0)
				a=0;
		}
	}
}
vector<double>& Convolution::operator[] (int i)
{
    if(i<self.size())
        return self[i];
    else
    {
        std::cout<<"矩阵范围溢出"<<std::endl;
        return self[0];
    }
}

class ConvolutionCore:public Convolution
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

Convolution convoluting(Convolution ori,ConvolutionCore core,int step)
{
    if((ori.h()-core.d()%step)!=0||(ori.w()-core.d()%step)!=0)
        ori.expand(step-(ori.h()-core.d()%step),step-(ori.w()-core.d()%step));
    Convolution result((ori.w()-core.d())/step+1,(ori.h()-core.d())/step+1);
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
#endif //KAWORU_TOOLSFORCNN_H
