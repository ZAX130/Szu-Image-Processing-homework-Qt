#pragma once
#include <opencv2/opencv.hpp>
#include<qdebug.h>
#include<iostream>
using namespace std;
class Utils
{
public:
	static cv::Mat fourierTransform(const cv::Mat& originImage); // ·µ»Ø¸µÀïÒ¶±ä»»Í¼Ïñ
	static cv::Mat LogfourierTransform(const cv::Mat& originImage);
	static cv::Mat expinverseFourierTransform(const cv::Mat& fourierImage);
	static cv::Mat getMagnitudeImage(const cv::Mat& fourierImage);  //µÃµ½·ùÖµÍ¼Ïñ
	static cv::Mat changeCenter(const cv::Mat& magImage);
	static cv::Mat getPhaseImage(const cv::Mat& fourierImage);
	static cv::Mat inverseFourierTransform(const cv::Mat& fourierImage);
	static cv::Mat butterworth_low_kernel(cv::Mat& scr, float sigma, int n);
	static cv::Mat getButterworth_low_kernel(cv::Mat& src, float d0, int n);
	static cv::Mat butterworth_high_kernel(cv::Mat& scr, float sigma, int n);
	static cv::Mat getButterworth_high_kernel(cv::Mat& src, float d0, int n);
	static cv::Mat ideal_low_kernel(cv::Mat& scr, float sigma);
	static cv::Mat getideal_low_kernel(cv::Mat& src, float d0);
	static cv::Mat ideal_high_kernel(cv::Mat& scr, float sigma);
	static cv::Mat getideal_high_kernel(cv::Mat& src, float d0);
	static cv::Mat homo_kernel(cv::Mat& scr, float gammaH, float gammaL, float sigma, int n);
	static cv::Mat gethomo_kernel(cv::Mat& src, float gammaH, float gammaL, float d0, int n);
	static cv::Mat filtered_fourier(cv::Mat& blur, const cv::Mat& fourierImage); // ·µ»Ø¸µÀïÒ¶±ä»»Í¼Ïñ
	static cv::Mat ToShowFormat(cv::Mat& image);
	static cv::Mat image_make_border(cv::Mat& src);
};

