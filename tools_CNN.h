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

class convolution
{
private:
	int width,hight;
	matrix beforePooling;
	void expand(int w,int h){self.expand(w,h);width=self.w();hight=self.h();}
	void expand(int w,int h,double n){self.expand(w,h,n);width=self.w();hight=self.h();}
public:
	matrix self;
	convolution()= default;
	convolution(int wd,int ht):width(wd),hight(ht){matrix temp(wd,ht);self=temp;}
	inline int w(){return width;}
	inline int h(){return hight;}
	inline int size(){return self.size();}
	void pooling(int d,string mode);
};
void convolution::pooling(int d,string mode)
{
	int modeCode;
	if(mode=="max")
		modeCode=0;
	else if(mode=="average")
		modeCode=1;
	if((width-d)%d!=0||(hight-d)%d!=0)
		expand((width-d)%d,(hight-d)%d);
	convolution temp((width-d)/d+1,(hight-d)/d+1);
	switch(modeCode)
	{
		case 0:
			for(int ix=0,kx=0;ix<=width-d;ix+=d,kx++)
			{
				for(int iy=0,ky=0;iy<=hight-d;iy+=d,ky++)
				{
					int max = 0;
					int max_x=0,max_y=0;
					for (int x = 0; x < d; x++)
					{
						for (int y = 0; y < d; y++)
						{
							if (self.self[ix + x][iy + y] > max)
							{
								max = self.self[ix + x][iy + y];
								max_x=ix+x;
								max_y=iy+y;
							}
						}
					}
					temp.self.self[kx][ky]=max;
					beforePooling.self[max_x][max_y]=max;
				}
			}
			break;
		case 1:
			for(int ix=0,kx=0;ix<=width-d;ix+=d,kx++)
			{
				for(int iy=0,ky=0;iy<=hight-d;iy+=d,ky++)
				{
					int sum;
					for (int x = 0; x < d; x++)
					{
						for (int y = 0; y < d; y++)
						{
								sum += self.self[ix + x][iy + y];
						}
					}
					temp.self.self[kx][ky]=sum/(d*d);
					for (int x = 0; x < d; x++)
					{
						for (int y = 0; y < d; y++)
						{
							beforePooling.self[ix + x][iy + y]=temp.self.self[kx][ky];
						}
					}
				}
			}

	}
}
#endif //KAWORU_TOOLSFORCNN_H
