#pragma once

using namespace cv;

namespace Dist
{
	static int ker_size = 77;
	static int max_ker_size = 200;

	int findRadius(Mat, Point*);
	void extractCircle(Mat*,int,Point*);

	Mat Dist(Mat frame, Point* max_loc,int* radius)
	{
		Mat dist;

		distanceTransform(frame, dist, CV_DIST_L2, 3);

		double min,max;
		Point min_loc;
		
		minMaxLoc(dist, &min, &max, &min_loc, max_loc);
		int result = findRadius(dist, max_loc);

		if (!result)
		{
			*radius = 0;
		}
		else
		{
			*radius = ker_size;
			extractCircle(&dist, *radius, max_loc);
		}

		normalize(dist, dist, 0, 1., NORM_MINMAX);


		cv::imshow("Distance", dist);

		return Mat(dist);
	}

	int findRadius(Mat frame, Point *max_loc)
	{
		vector<int> vec(4);


		Size s = frame.size();

		vec[0] = max_loc->x;
		vec[1] = max_loc->y;
		vec[2] = frame.cols - vec[0];
		vec[3] = frame.rows - vec[1];

		int min = vec[0];
		for (int i = 1; i < 4; i++)
		{
			if (vec[i] < min)
			{
				min = vec[i];
			}
		}
		
		if (ker_size + 1 >= min)
		{
			cout << "Kernel size too big! (Dist.h)\n";
		}
		else
		{
			int min_col = vec[0] - ker_size;
			int max_col = vec[0] + ker_size;
			int min_row = vec[1] - ker_size;
			int max_row = vec[1] + ker_size ;
			
			for (int i = min_row; i < vec[1]; i++)
			{
				if (frame.at<int>(i,min_col) < 0.0001)
				{
					return 1;
				}
				if (frame.at<int>(i,max_col) < 0.0001)
				{
					return 1;
				}
			}

			for (int j = min_col; j < vec[0]; j++)
			{
				if (frame.at<int>(Point(min_row,j)) < 0.0001)
				{
					return 1;
				}
			}

			return 1;
		}

		return 0;
	}

	void extractCircle(Mat* frame, int radius, Point *max_loc)
	{
		for (int i = max_loc->y; i < frame->rows; i++)
			for (int j = 0; j < frame->cols; j++)
				frame->at<int>(i, j) = 0;

		for (int i = max_loc->y - radius; i < max_loc->y; i++)
		{
			for (int j = max_loc->x - radius; j < max_loc->x + radius; j++)
			{
				if ((i - max_loc->y) ^ 2 + (j - max_loc->x) ^ 2 - radius ^ 2 < 0.0001)
				{
					frame->at<int>(i, j) = 0;
				}
			}
		}
	}
}