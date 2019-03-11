//
// Created by nagi on 19-3-10.
//

#ifndef KAWORU_TOOLS_FILE_H
#define KAWORU_TOOLS_FILE_H
#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include "tools_general.h"

using std::vector;
using cv::Mat;
using cv::Mat_;

matrix getimg_grayscale(char *path)
{
	Mat image = cv::imread(path, 0);
	if(image.empty())
		fprintf(stderr, "无法加载图片 %s\n", path);
	int w = image.cols;
	int h = image.rows;
	matrix target(h,w);
	Mat_<uchar>::iterator it = image.begin<uchar>();
	for(int x=0;x<w;x++)
	{
		for(int y=0;y<h;y++)
		{
			target.self[x][y] = *(it + y * w + x);
		}
	}
	return target;
}

#endif //KAWORU_TOOLS_FILE_H
