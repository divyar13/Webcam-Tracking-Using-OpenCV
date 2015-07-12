#include <opencv2/core/core.hpp>
#include <opencv/cv.h>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <Windows.h>

#define USE_VGA

using namespace cv;
using namespace std;

float MAXVAL(float A, float B, float C)
{
	return (A>B) ? (A>C ? A:C):(B>C ? B:C);
}

float MINVAL(float A, float B, float C)
{
	return (A<B) ? (A<C ? A:C):(B<C ? B:C);
}


int main()
{
	char key;
	char buffer[50]={0};
	
	static int posX = 0;

    static int posY = 0;


	CvCapture* capture = cvCaptureFromCAM(CV_CAP_ANY);
	int count=1;
	char buff[50]={0};
	int ht, wd;

	int val1 = 0, val2 = 0, val3 = 0;
	
	char k;
	cout << "Starting now" << endl;
	
#ifdef USE_VGA
		Mat drawing = Mat::zeros(480, 640, CV_8UC3);
		drawing = Scalar(255,255,255);
#else
		Mat drawing = Mat::zeros(768, 1366, CV_8UC3);
		drawing = Scalar(255,255,255);
#endif		
		
		namedWindow("cameraOutput",1);
		namedWindow("canvas",1);


		while(1)
		{
			
			Mat frame = cvQueryFrame(capture);
			Mat frameHSV;//(frame.rows,frame.cols,CV_32F);
			frame.convertTo(frameHSV, CV_32F);

			cvtColor(frame,frameHSV,CV_BGR2HSV);
			//BGR2HSV(frame, frameHSV);
			
			

#ifdef DUMP_IMAGES
			sprintf(buff,"webcam_%d.png",count);
			imwrite(buff,frame);
			sprintf(buff,"webcamHSV_%d.png",count);
			imwrite(buff,frameHSV);
#endif
			
			ht = frame.rows;
			wd = frame.cols;

			Mat imgThresh1 = Mat::zeros(frameHSV.rows, frameHSV.cols, CV_8UC1);
			Mat imgThresh2 = Mat::zeros(frameHSV.rows, frameHSV.cols, CV_8UC1);
			Mat imgThresh = Mat::zeros(frameHSV.rows, frameHSV.cols, CV_8UC1);
			//inRange(frameHSV, Scalar(114, 135, 135), Scalar(142, 255, 255), imgThresh1);
			Mat imgHSV = frameHSV;
		
			inRange(frameHSV, Scalar(159, 135, 135), Scalar(179, 255, 255), imgThresh);
			imshow("cameraOutput",frame);
			
#ifdef DUMP_IMAGES
			char b1[50] = {0};
			char b2[50] = {0};
			char b3[50] = {0};
			sprintf(b1, "t1_%d.png", count);
			sprintf(b2, "t2_%d.png", count);
			sprintf(b3, "thresholded_%d.png", count);

			imwrite(b1, imgThresh1);
			imwrite(b2, imgThresh2);
			imwrite(b3, imgThresh);
#endif
			key = waitKey(10);
			if((char)key == 27) //Esc key
				break;

			if(key == 'n') //refresh the canvas
			{
				sprintf(buffer,"drawing_%d.png",count);
				count++;
				imwrite(buffer,drawing);
				printf("Your drawing has been saved --> %s\n",buffer);

				drawing = Scalar(255,255,255);
				
			}

			if(key == 'g') //refresh the canvas
			{
				sprintf(buffer,"drawing_%d.png",count);
				count++;
				imwrite(buffer,drawing);
				printf("Your drawing has been saved --> %s\n",buffer);

				drawing = Scalar(255,255,255);
				val1 = 0;
				val2 = 160;
				val3 = 0;
				
			}

			if(key == 'b') //refresh the canvas
			{
				sprintf(buffer,"drawing_%d.png",count);
				count++;
				imwrite(buffer,drawing);
				printf("Your drawing has been saved --> %s\n",buffer);

				drawing = Scalar(255,255,255);
				val1 = 100;
				val2 = 0;
				val3 = 0;
				
			}

			if(key == 'r') //refresh the canvas
			{
				sprintf(buffer,"drawing_%d.png",count);
				count++;
				imwrite(buffer,drawing);
				printf("Your drawing has been saved --> %s\n",buffer);

				drawing = Scalar(255,255,255);
				val1 = 0;
				val2 = 0;
				val3 = 255;
				
			}

			

			// Calculate the moments to estimate the position of the ball
			Moments ourMoment; //moments variable
			ourMoment=moments(imgThresh); 
			double moment10=ourMoment.m10; //extract spatial moment 10
			double moment01=ourMoment.m01; //extract spatial moment 01
			double area=ourMoment.m00;

			// Holding the last and current ball positions
        
			int lastX = posX;
			int lastY = posY;

#ifdef USE_VGA
			posX = moment10/area;
			posY = moment01/area;
#else
			posX = 2.13 * moment10/area;
			posY = 1.6 * moment01/area;
#endif

			//cout << "posX: " << posX <<"\nposY: " << posY << endl;

			if(posX<0 && posY<0)
			{
				posX = lastX;
				posY = lastY;
			}

			if(lastX>0 && lastY>0 && posX>0 && posY>0)
			{
				line(drawing, cvPoint(posX, posY), cvPoint(lastX, lastY), cvScalar(val1,val2,val3), 5);
				//SetCursorPos(2.13*posX, 1.6*posY); //To control the cursor (instead of drawing)

			}
			imshow("canvas",drawing);
			
		}
		cvReleaseCapture(&capture);
		cvDestroyWindow("cameraOutput");
		
		sprintf(buffer,"drawing_%d.png",count);
		imwrite(buffer,drawing);
		printf("Your drawing has been saved --> %s\n\n",buffer);
		printf("Your drawings have been saved\n");

	return 0;
}