#include "Blob.h"
#include <iostream>
#include <stdint.h>
#include<conio.h>           
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <opencv2/imgcodecs.hpp>

#define SHOW_STEPS            
using namespace std;
using namespace cv;
// ///////////////////////////////////////////////////////////////////////////////
const cv::Scalar SCALAR_BLACK = cv::Scalar(0.0, 0.0, 0.0);
const cv::Scalar SCALAR_RED = cv::Scalar(0.0, 0.0, 255.0);
const cv::Scalar SCALAR_YELLOW = cv::Scalar(0.0, 255.0, 255.0);
const cv::Scalar SCALAR_GREEN = cv::Scalar(0.0, 200.0, 0.0);
const cv::Scalar SCALAR_WHITE = cv::Scalar(0.0, 0.0, 255.0);

// ////////////////////////////////////////////////////////////////////////////
int main(void) {
	cv::VideoCapture capVideo;

	cv::Mat imgFrame1;
	cv::Mat imgFrame2;
	cv::Mat imgFrame3;
	std::vector<Blob> blobs;

	cv::Point crossingLine[4];

	int carCount = 0;
	string s;
	//cout << "¬ведите им€ файла "<<"\n";
	//cin >> s;
	capVideo.open("video(1).mp4");

	if (!capVideo.isOpened()) {                                                 // if unable to open video file
		std::cout << "error reading video file" << std::endl << std::endl;      // show error message
		_getch();                   // it may be necessary to change or remove this line if not using Windows
		return(0);                                                              // and exit program
	}

	if (capVideo.get(CAP_PROP_FRAME_COUNT) < 2) {
		std::cout << "error: video file must have at least two frames";
		_getch();                   // it may be necessary to change or remove this line if not using Windows
		return(0);
	}

	capVideo.read(imgFrame1);
	capVideo.read(imgFrame2);
	capVideo.read(imgFrame3);
	
	//cout << "¬ведите координаты по горизонтале линии 1" << "\n";
	//cin >> y;
	//cout << "¬ведите координаты по горизонтале линии 2" << "\n";
	//cin >> y2;


	char chCheckForEscKey = 0;//проверка на выход

	bool blnFirstFrame = true;

	int frameCount = 2;
	while (capVideo.isOpened()) {
		std::vector<Blob> currentFrameBlobs;

		cv::Mat imgFrame1Copy = imgFrame1.clone();//создаем копию изображени€
		cv::Mat imgFrame2Copy = imgFrame2.clone();//создаем копию изображени€
	//	cv::Mat imgFrame3Copy = imgFrame3.clone();//создаем копию изображени€

		cv::Mat imgDifference;
		cv::Mat imgDifference2;

		cv::Mat imgThresh;

		cv::cvtColor(imgFrame1Copy, imgFrame1Copy, COLOR_BGR2GRAY);//переделываение в серый цвет
		cv::cvtColor(imgFrame2Copy, imgFrame2Copy, COLOR_BGR2GRAY);
		//	cv::cvtColor(imgFrame3Copy, imgFrame3Copy, COLOR_BGR2GRAY);

		cv::GaussianBlur(imgFrame1Copy, imgFrame1Copy, cv::Size(5, 5), 0);
		cv::GaussianBlur(imgFrame2Copy, imgFrame2Copy, cv::Size(5, 5), 0);
		//	cv::GaussianBlur(imgFrame3Copy, imgFrame3Copy, cv::Size(5, 5), 0);

		cv::absdiff(imgFrame1Copy, imgFrame2Copy, imgDifference);
		//	cv::absdiff(imgFrame1Copy, imgFrame3Copy, imgDifference2);

		cv::threshold(imgDifference, imgThresh, 18, 255.0, THRESH_BINARY);//

		cv::imshow("imgThresh", imgThresh);//

		cv::Mat structuringElement3x3 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
		cv::Mat structuringElement5x5 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
		cv::Mat structuringElement7x7 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(7, 7));
		cv::Mat structuringElement15x15 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(15, 15));

		for (unsigned int i = 0; i < 2; i++) {
			cv::dilate(imgThresh, imgThresh, structuringElement5x5);
			cv::dilate(imgThresh, imgThresh, structuringElement5x5);
			cv::erode(imgThresh, imgThresh, structuringElement5x5);
		}

		cv::Mat imgThreshCopy = imgThresh.clone();

		std::vector<std::vector<cv::Point> > contours;

		cv::findContours(imgThreshCopy, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE); //выделение контуров целиком
		
		

	

		imgFrame2Copy = imgFrame2.clone();          // создаем копию кадра

	

		resize(imgFrame2Copy, imgFrame2Copy, cv::Size(), 0.60, 0.6);

		cv::imshow("imgFrame2Copy", imgFrame2Copy);//выводим на экран
			   // uncomment this line to go frame by frame for debugging

		// now we prepare for the next iteration
		currentFrameBlobs.clear();

		imgFrame1 = imgFrame2.clone();           // move frame 1 up to where frame 2 is
		capVideo.read(imgFrame2);
		if ((capVideo.get(CAP_PROP_FRAME_COUNT) + 1) < capVideo.get(CAP_PROP_FRAME_COUNT))
		{
			waitKey(27);
			capVideo.read(imgFrame2);
		}		//else {
		//std::cout << "end of video\n";
		//break;
//	}

		blnFirstFrame = false;
		frameCount++;
		chCheckForEscKey = cv::waitKey(110);
	
	}
	if (chCheckForEscKey != 27) {               // if the user did not press esc (i.e. we reached the end of the video)
		cv::waitKey(0);                         // hold the windows open to allow the "end of video" message to show
	}
	// note that if the user did press esc, we don't need to hold the windows open, we can simply let the program end which will close the windows

	return(0);
}