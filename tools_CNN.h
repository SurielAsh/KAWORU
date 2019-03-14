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

class Convolution
{
private:
	int width,hight;
	Matrix beforePooling;
	void expand(int w,int h){self.expand(w,h);width=self.w();hight=self.h();}
	void expand(int w,int h,double n){self.expand(w,h,n);width=self.w();hight=self.h();}
public:
	Matrix self;
	Convolution()= default;
	Convolution(int wd,int ht):width(wd),hight(ht){Matrix temp(wd,ht);self=temp;}
	inline int w(){return width;}
	inline int h(){return hight;}
	inline int size(){return self.size();}
	Convolution pooling(int d,string mode);
	Convolution convoluting();
};
Convolution Convolution::pooling(int d,string mode)
{
	Convolution return_temp=*this;
	int modeCode;
	if(mode=="max")
		modeCode=0;
	else if(mode=="average")
		modeCode=1;
	if((return_temp.width-d)%d!=0||(return_temp.hight-d)%d!=0)
		expand((return_temp.width-d)%d,(return_temp.hight-d)%d);
	Convolution temp((return_temp.width-d)/d+1,(return_temp.hight-d)/d+1);
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

	}
	return return_temp;
}

class ConvolutionCore:public Convolution
{
private:
	int dimension;
public:
	ConvolutionCore()= default;
	ConvolutionCore(int d):dimension(d){Matrix temp(d,d);self=temp;}
	inline int d(){return dimension;}
	void coreInitialize(int absoluteRage);
};

void ConvolutionCore::coreInitialize(int absoluteRage)
{
	for(auto &v:self)
	{
		for(auto &a:v)
		{
			a=rdm(2*absoluteRage)-absoluteRage;
		}
	}
}
#endif //KAWORU_TOOLSFORCNN_H
