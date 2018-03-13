#pragma once
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>

using namespace cv;

namespace CannyH
{
	Mat canny_img, detected_edges;

	static int edgeTresh = 1;
	static int lowThresh;
	static int ratio = 3;
	const int max_lowThresh = 100;
	static int kernel_size = 3;

	Mat CannyThresh(Mat frame)
	{
		canny_img.create(frame.size(), frame.type());

		blur(frame, detected_edges, Size(3, 3));

		Canny(detected_edges, detected_edges, lowThresh,
			lowThresh*ratio, kernel_size);

		canny_img = Scalar::all(0);

		frame.copyTo(canny_img, detected_edges);
		imshow("Edges", canny_img);

		return Mat(frame);
	}
}