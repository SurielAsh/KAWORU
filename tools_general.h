//
// Created by nagi on 19-3-10.
//

#ifndef KAWORU_GENERALTOOLS_H
#define KAWORU_GENERALTOOLS_H
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <sys/time.h>

using std::vector;

double rdm(int n)
{
	srand(clock()+rand());
	return rand()%(100*n)/100;
}
double rdm(int n,int presision)
{
	srand(clock()+rand());
	int k=pow(10,presision);
	return rand()%(k*n)/k;
}
template <typename T>
void vini(vector<T> &v,T par,int n)
{
	for(int i=0;i<n;i++)
		v.push_back(par);
}
template <typename T>
void vini(vector<vector<T>> &v,vector<T> &t,int n)
{
	for(int i=0;i<n;i++)
		v.push_back(t);
}
class matrix{
private:
	vector<double> row;
	vector<double> column;
	int hight,width;
public:
	vector<vector<double>> self;
	matrix()=default;
	matrix(int ht,int wd,int mode);
	matrix(int ht,int wd):hight(ht),width(wd){vini(row,0.,wd);vini(self,row,ht);}
	inline int h(){return hight;}
	inline int w(){return width;}
	inline int size(){return hight*width;}
	matrix T();
	matrix rot180();
};
matrix::matrix(int ht,int wd,int mode):hight(ht),width(wd)
{
	if(mode==0)
	{
		srand(clock()+rand());
		vini(row,0.,wd);
		vini(self,row,ht);
		for(auto &r:self)
		{
			for(auto &a:r)
			{
				a=rand()%3-1;
			}
		}
	}
	else
	{
		vini(row,(double)mode,wd);
		vini(self,row,ht);
	}
};
matrix matrix::T()
{
	matrix temp(hight,width);
	for(int x;x<width;x++)
	{
		for(int y;y<hight;y++)
		{	for(int x;x<width;x++)
			{
				for(int y;y<hight;y++)
				{
					temp.self[x][y]=self[y][x];
				}
			}
			temp.self[x][y]=self[y][x];
		}
	}
	return temp;
}
matrix matrix::rot180()
{
	matrix temp(hight,width);
	for(int x;x<width;x++)
	{
		for(int y;y<hight;y++)
		{
			temp.self[x][y]=self[width-1-x][hight-1-y];
		}
	}
	return temp;
}
#endif //KAWORU_GENERALTOOLS_H
