#pragma once

#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv\highgui.h>
#include <stdlib.h>
#include <iostream>

using namespace cv;
using namespace std;

namespace ERDL
{
	Mat src_img, erosion_img, dilation_img;

	static int erosion_elem = 0;
	static int erosion_size = 3;
	static int dilation_elem = 0;
	static int dilation_size = 0;
	const int max_elem = 2;
	const int max_kernel_size = 21;

	void Erosion();
	void Dilation();

	Mat ERDL(Mat frame)
	{
		src_img = frame;

		//Dilation();
		Erosion();

		//waitKey(0);
		return Mat(src_img);
	}

	/**  @function Erosion  */
	void Erosion()
	{
		int erosion_type;
		if (erosion_elem == 0) { erosion_type = MORPH_RECT; }
		else if (erosion_elem == 1) { erosion_type = MORPH_CROSS; }
		else if (erosion_elem == 2) { erosion_type = MORPH_ELLIPSE; }

		Mat element = getStructuringElement(erosion_type,
			Size(2 * erosion_size + 1, 2 * erosion_size + 1),
			Point(erosion_size, erosion_size));

		/// Apply the erosion operation
		erode(src_img, erosion_img, element);
		src_img = erosion_img;
		cv::imshow("Erosion", erosion_img);
	}

	/** @function Dilation */
	void Dilation()
	{
		int dilation_type;
		if (dilation_elem == 0) { dilation_type = MORPH_RECT; }
		else if (dilation_elem == 1) { dilation_type = MORPH_CROSS; }
		else if (dilation_elem == 2) { dilation_type = MORPH_ELLIPSE; }

		Mat element = getStructuringElement(dilation_type,
			Size(2 * dilation_size + 1, 2 * dilation_size + 1),
			Point(dilation_size, dilation_size));
		/// Apply the dilation operation
		dilate(src_img, dilation_img, element);
		src_img = dilation_img;
		cv::imshow("Dilation", dilation_img);
	}
}