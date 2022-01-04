#pragma once
#include <opencv2/opencv.hpp>

class Utils
{
public :
	static cv::Mat fourierTransform(const cv::Mat& originImage); // 返回傅里叶变换图像
	static cv::Mat getMagnitudeImage(const cv::Mat& fourierImage);  //得到幅值图像
	static cv::Mat changeCenter(const cv::Mat& magImage);
	static cv::Mat getPhaseImage(const cv::Mat& fourierImage);
	static cv::Mat inverseFourierTransform(const cv::Mat& fourierImage);
};

