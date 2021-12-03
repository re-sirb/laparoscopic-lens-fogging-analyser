#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

#include <iostream>

using namespace cv;
using namespace std;

void rearrange(Mat &magI)
{

	magI = magI(Rect(0, 0, magI.cols & -2, magI.rows & -2));
	int cx = magI.cols/2;
	int cy = magI.rows/2;
	
	Mat q0(magI, Rect(0, 0, cx, cy)); 
	Mat q1(magI, Rect(cx, 0, cx, cy)); 
	Mat q2(magI, Rect(0, cy, cx, cy));  
	Mat q3(magI, Rect(cx, cy, cx, cy)); 
	
	Mat tmp;
	q0.copyTo(tmp);
	q3.copyTo(q0);
	tmp.copyTo(q3);
	
	q1.copyTo(tmp);   
	q2.copyTo(q1);
	tmp.copyTo(q2);
}


void highpassFilter(Mat &dft_Filter, int distance)
{

    Mat tmp = Mat(dft_Filter.rows, dft_Filter.cols, CV_32F);
    Point centre = Point(dft_Filter.rows / 2, dft_Filter.cols / 2);
    double radius;

    for(int i = 0; i < dft_Filter.rows; i++)
    {
        for(int j = 0; j < dft_Filter.cols; j++)
        {
            radius = (double) sqrt(pow((i - centre.x), 2.0) + pow((double) (j - centre.y), 2.0));
            if(radius>distance){
                tmp.at<float>(i,j) = (float)1;
            }else{
                tmp.at<float>(i,j) = (float)0;
            }
        }
    }
    Mat toMerge[] = {tmp, tmp};
    merge(toMerge, 2, dft_Filter);
}

double analyse_image(Mat &I)
{
	
	Mat padded;                           
	int m = getOptimalDFTSize( I.rows );
	int n = getOptimalDFTSize( I.cols ); 
	copyMakeBorder(I, padded, 0, m - I.rows, 0, n - I.cols, BORDER_CONSTANT, Scalar::all(0));
	
	
	Mat planes[] = {Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F)};
	Mat complexI;
	
	merge(planes, 2, complexI);  
	
	dft(complexI, complexI); 
	
	split(complexI, planes);
	
	rearrange(planes[0]);
	rearrange(planes[1]);
	
	merge(planes, 2, complexI);
	
	Mat filter = complexI.clone();
	highpassFilter(filter, 60);
	mulSpectrums(complexI, filter, complexI, 0);
	
	split(complexI, planes);
	
	rearrange(planes[0]);
	rearrange(planes[1]);
	
	merge(planes, 2, complexI);
	
	dft(complexI, complexI, DFT_INVERSE + DFT_SCALE);
	
	split(complexI, planes); 
	magnitude(planes[0], planes[1], planes[0]);
	Mat magI = planes[0];
	
	magI += Scalar::all(0.0001);
	log(magI, magI);
	
	magI=magI*20;
	
	cv::Scalar ret_val = cv::mean(magI);
	double temp_val=ret_val[0];
	//cout << ret_val[0] << endl;
	return (temp_val);
}

double analyse_video_stream(string file_path){
	
	double output_val=0.0;
	int output_counter=0;
	VideoCapture cap(file_path); 
	if(!cap.isOpened()){
	cout << "Error opening video file" << endl;
	return -1;
	}
	
	int video_length = cap.get(cv::CAP_PROP_FRAME_COUNT);
	
	while(1){
	
	Mat frame, gray, resized_frame;
	cap >> frame;
	if (frame.empty()) break;
	
	cv::cvtColor(frame, gray, CV_BGR2GRAY);
	
	cv::Size s = gray.size();
	int rows = s.height;
	int cols = s.width;
	if(cols > 400)
	{
		float scale_factor=400.0/(float)cols;
		resize(gray, resized_frame, Size(), scale_factor, scale_factor, CV_INTER_LINEAR );
	}
	else
	{
		resized_frame=gray;
	}
	

	
	output_val=output_val+ analyse_image(resized_frame);
	output_counter=output_counter+1;
	
	}
	cap.release();
	
	if(output_counter>0){
		output_val=output_val/(double)output_counter;
	}
	else{
		output_val=0.0;
	}
	
	return (output_val);
}

int main(int argc, char ** argv)
{
	
	if(argc==2)
	{
		string filename = argv[1];
		cout << "Analysing fogging in video file: " << filename << endl;
		double a = analyse_video_stream(filename);
		cout << a << endl;
		return 0;
	} 
	else if(argc>2)
	{
		cout << "Too many arguments given!" << endl;
		cout << endl;
		cout << "video_fogging_analyser [video_file]" << endl;
		cout << endl;
		return -1;
	}
	else
	{
		cout << "A video file must be given as argument!" << endl;
		cout << endl;
		cout << "video_fogging_analyser [video_file]" << endl;
		cout << endl;
		return -1;
	}
}
