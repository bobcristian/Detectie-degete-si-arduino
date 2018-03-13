#include"ERDL.h"
#include"CannyH.h"
#include"Hull.h"
#include"Dist.h"

#include<opencv2\imgcodecs.hpp>
#include<opencv2\imgproc.hpp>
#include<opencv2\videoio.hpp>
#include<opencv2\highgui.hpp>
#include<opencv2\video.hpp>
#include<opencv2\features2d.hpp>
#include<opencv2\features2d\features2d.hpp>
#include<opencv2\core.hpp>
#include<opencv2\flann\miniflann.hpp>

#include<iostream>
#include<sstream>
#include<stdlib.h>
#include<ctime>
#using<System.dll>

using namespace System::IO::Ports;
using namespace std;
using namespace cv;
using namespace ERDL;
using namespace CannyH;
using namespace Hull;
using namespace Dist;


Mat frame, imKey;
Mat ROI;
Mat fgMaskMOG2;
Mat Eros, can, CH, dist;
Ptr<BackgroundSubtractor> pMOG2;
char key;

cv::Point pt1, pt2;
bool oncePressed = false;
bool gotROI = false;

void mouse_click(int e, int x, int y, int flags, void* param)
{
	//Trebuie aplicat doar de la stanga la dreapta in jos
	switch (e)
	{
	case CV_EVENT_LBUTTONDOWN:
	{
		if (!oncePressed)
		{
			pt1.x = x;
			pt1.y = y;
			oncePressed = true;
			gotROI = false;
		}
		else
		{
			cv::Point auxPt(x, y);
			rectangle(frame, cv::Rect(pt1, auxPt), cv::Scalar(255, 255, 255),3);
		}
		break;
	}
	case(CV_EVENT_LBUTTONUP):
	{
		pt2.x = x;
		pt2.y = y;

		oncePressed = false;
		gotROI = true;
		break;
	}
	}
}

void createTrackbars()
{
	namedWindow("Erosion");
	//namedWindow("Dilation");

	createTrackbar("Element:\n 0: Rect \n 1: Cross \n 2: Ellipse", "Erosion",
		&erosion_elem, max_elem);

	createTrackbar("Kernel size:\n 2n +1", "Erosion",
		&erosion_size, max_kernel_size);

	/// Create Dilation Trackbar
	//createTrackbar("Element:\n 0: Rect \n 1: Cross \n 2: Ellipse", "Dilation",
		//&dilation_elem, max_elem);

	//createTrackbar("Kernel size:\n 2n +1", "Dilation",
		//&dilation_size, max_kernel_size);

	///Create Edges trackbar
	//createTrackbar("Min Threshold:", "Edges"
		//, &lowThresh, max_lowThresh);

	///Create Hull trackbar
	createTrackbar(" Threshold:", "Hull",
		&thresh, max_thresh);

	///Create Distance trackbar
	createTrackbar("Kernel size: ", "Distance",
		&Dist::ker_size, Dist::max_ker_size);
}

void extractCircle2(Mat* frame, int radius, Point *max_loc)
{
	for (int i = max_loc->y; i < frame->rows; i++)
		for (int j = 0; j < frame->cols; j++)
			frame->at<unsigned char>(i, j) = 0;

	for (int i = max_loc->y - radius; i < max_loc->y; i++)
	{
		for (int j = max_loc->x - radius; j < max_loc->x + radius; j++)
		{
			//if ((i - max_loc->y) ^ 2 + (j - max_loc->x) ^ 2 - radius ^ 2 < 0.0001)
			//{
				frame->at<unsigned char>(i, j) = 0;
			//}
		}
	}
}
int main()
{
	namedWindow("Blobs");
	//namedWindow("Edges");
	namedWindow("Hull");
	namedWindow("Distance");
	namedWindow("Frame");
	namedWindow("FG Mask MOG2");
	createTrackbars();
	
	///Connect to Arduino
	SerialPort^ arduino = gcnew SerialPort("COM3", 9600, Parity::None, 8, StopBits::One);
	arduino->Open();
	arduino->Write("-1");
	//arduino->Close();


	pMOG2 = createBackgroundSubtractorMOG2();

	VideoCapture cap(0);
	if (!cap.isOpened())
	{
		cerr << "Unable to open camera!\n";
		return EXIT_FAILURE;
	}

	cvSetMouseCallback("Frame", mouse_click, 0);

	//Starting processing
	key = 0;
	std::clock_t start;
	start = std::clock();

	while (key != 'q' && key != 27)
	{
		if (!cap.read(frame))
		{
			cerr << "Error to read frame!\n";
			exit(EXIT_FAILURE);
		}

		if (!gotROI)
		{
			//Wait to get ROI
		}
		else
		{
			pMOG2->apply(frame(Range(pt1.y, pt2.y), Range(pt1.x, pt2.x)),
				fgMaskMOG2,0);

			Eros = ERDL::ERDL(fgMaskMOG2);
			//can = CannyH::CannyThresh(Eros);
			//CH = Hull::Hull(Eros);
			
			Point max_loc;
			int radius;
			dist = Dist::Dist(Eros,&max_loc,&radius);

			if (radius)
			{
				extractCircle2(&Eros, radius, &max_loc);
				cv::imshow("Erosion", Eros);
			}

			double duration = (std::clock() - start) / (double) CLOCKS_PER_SEC;
			if (duration > 5.0)
			{
				int fingers = Hull::Hull(Eros) - 1;
				arduino->Write(System::Convert::ToString(fingers));
				start = std::clock();
			}

			max_loc = max_loc + pt1;
			cv::circle(frame, max_loc, radius, Scalar(255, 255, 0));
			
			stringstream ss;
			rectangle(frame, cv::Point(10, 2), cv::Point(100, 20),
				cv::Scalar(255, 255, 255));
			putText(frame,std::to_string(5-duration), cv::Point(15, 15),
				FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0));

			//Show Images
			cv::imshow("FG Mask MOG2", fgMaskMOG2);
		}
		cv::imshow("Frame", frame);

		key = (char)waitKey(30);
	}
	
	//Release Memory
	cap.release();
	cvDestroyAllWindows();
	arduino->Close();

	return 0;
}