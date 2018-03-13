#pragma once
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace cv;
using namespace std;

namespace Hull
{
	static int thresh = 100;
	const int max_thresh = 255;
	RNG rng(12345);

	int Hull(Mat frame)
	{
		Mat src = frame;
		Mat thresh_out;
		vector<vector<Point>> countours;
		vector<Vec4i> hierarchy;

		findContours(src, countours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

		cout << hierarchy.size() << endl;

		vector<vector<Point>> hull(countours.size());

		for (int i = 0; i < countours.size(); i++)
		{
			convexHull(Mat(countours[i]), hull[i]);
		}

		//Draw countours
		Mat drawing = Mat::zeros(src.size(), CV_8UC3);
		for (int i = 0; i < countours.size(); i++)
		{
			drawContours(drawing, countours, i, Scalar(0, 0, 255), 1, 8,
				vector<Vec4i>(), 0, Point());
			drawContours(drawing, hull, i, Scalar(0,255,0), 1, 8,
				vector<Vec4i>(), 0, Point());
		}

		//Dispaly image
		cv::imshow("Hull", drawing);

		return hierarchy.size();	
	}
}