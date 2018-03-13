#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv\highgui.h>
#include <stdlib.h>
#include <iostream>

using namespace cv;
using namespace std;

Mat src_img, erosion_img, dilation_img,ED_img;

int erosion_elem = 0;
int erosion_size = 0;
int dilation_elem = 0;
int dilation_size = 0;
const int max_elem = 2;
const int max_kernel_size = 21;

void Erosion(int, void*);
void Dilation(int, void*);

int Main1()
{
	//Load Image
	src_img = imread("mana.bmp");

	if (!src_img.data)
	{
		return -1;
	}

	namedWindow("Erosion", CV_WINDOW_AUTOSIZE);
	namedWindow("Dilation", CV_WINDOW_AUTOSIZE);

	createTrackbar("Element:\n 0: Rect \n 1: Cross \n 2: Ellipse", "Erosion",
		&erosion_elem, max_elem,
		Erosion);

	createTrackbar("Kernel size:\n 2n +1", "Erosion",
		&erosion_size, max_kernel_size,
		Erosion);

	/// Create Dilation Trackbar
	createTrackbar("Element:\n 0: Rect \n 1: Cross \n 2: Ellipse", "Dilation",
		&dilation_elem, max_elem,
		Dilation);

	createTrackbar("Kernel size:\n 2n +1", "Dilation",
		&dilation_size, max_kernel_size,
		Dilation);

	/// Default start
	Erosion(0, 0);
	Dilation(0, 0);

	waitKey(0);
	return 0;
}

/**  @function Erosion  */
void Erosion(int, void*)
{
	int erosion_type;
	if (erosion_elem == 0) { erosion_type = MORPH_RECT; }
	else if (erosion_elem == 1) { erosion_type = MORPH_CROSS; }
	else if (erosion_elem == 2) { erosion_type = MORPH_ELLIPSE; }

	Mat element = getStructuringElement(erosion_type,
		Size(2 * erosion_size + 1, 2 * erosion_size + 1),
		Point(erosion_size, erosion_size));

	/// Apply the erosion operation
	if (!ED_img.data)
	{
		erode(src_img, erosion_img, element);
	}
	else
	{
		erode(ED_img, erosion_img, element);
	}
	imshow("Erosion Demo", erosion_img);
}

/** @function Dilation */
void Dilation(int, void*)
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
	ED_img = dilation_img;
	imshow("Dilation Demo", dilation_img);
}