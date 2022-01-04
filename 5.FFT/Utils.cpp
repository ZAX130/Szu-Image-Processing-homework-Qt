#include "Utils.h"

cv::Mat Utils::fourierTransform(const cv::Mat& originImage) {
	cv::Mat paddingImage;

	//得到适合傅里叶变换的最优尺寸
	int m = cv::getOptimalDFTSize(originImage.rows);
	int n = cv::getOptimalDFTSize(originImage.cols);

	//填充，上侧和左侧不填充
	cv::copyMakeBorder(originImage, paddingImage, 0, m - originImage.rows,
		0, n - originImage.cols, cv::BORDER_CONSTANT, cv::Scalar(0));

	//双通道Mat
	cv::Mat planes[] = { cv::Mat_<float>(paddingImage), cv::Mat::zeros(paddingImage.size(), CV_32FC1) };
	cv::Mat mergeImage;
	cv::merge(planes, 2, mergeImage);

	//进行傅里叶变换
	cv::dft(mergeImage, mergeImage, cv::DFT_COMPLEX_OUTPUT);

	return mergeImage;
}

cv::Mat Utils::changeCenter(const cv::Mat& magImage) {
	//重新排布傅里叶变换后的图像，使得原点位于图像中心
	int centerX = magImage.cols / 2;
	int centerY = magImage.rows / 2;

	cv::Mat magImageCopy = magImage.clone();
	cv::Mat planes[] = { cv::Mat::zeros(magImageCopy.size(), CV_32FC1),
						cv::Mat::zeros(magImageCopy.size(), CV_32FC1) };

	cv::Mat mat1(magImageCopy, cv::Rect(0, 0, centerX, centerY));				//左上
	cv::Mat mat2(magImageCopy, cv::Rect(0, centerY, centerX, centerY));			//右上
	cv::Mat mat3(magImageCopy, cv::Rect(centerX, 0, centerX, centerY));				//左下
	cv::Mat mat4(magImageCopy, cv::Rect(centerX, centerY, centerX, centerY));	//右下

	//互换左上和右下
	cv::Mat tempImage;
	mat1.copyTo(tempImage);
	mat4.copyTo(mat1);
	tempImage.copyTo(mat4);

	//互换左下和右上
	mat2.copyTo(tempImage);
	mat3.copyTo(mat2);
	tempImage.copyTo(mat3);

	return magImageCopy;
}

cv::Mat Utils::getMagnitudeImage(const cv::Mat& fourierImage) {
	cv::Mat planes[] = { cv::Mat::zeros(fourierImage.size(), CV_32FC1),
						cv::Mat::zeros(fourierImage.size(), CV_32FC1) };

	cv::Mat magImage = planes[0].clone();
	cv::split(fourierImage, planes);
	cv::magnitude(planes[0], planes[1], magImage);

	//如果有奇数行或列，则转换为偶数
	magImage = magImage(cv::Rect(0, 0, magImage.cols - (magImage.cols % 2), magImage.rows - (magImage.rows % 2)));


	magImage = changeCenter(magImage);

	//计算幅值,并转换到对数尺度
	//取对数的目的是使那些振幅较低的成分相对高振幅成分得以拉高，以便观察掩盖在低幅噪声中的周期信号。
	cv::log(magImage + 1, magImage);

	//归一化处理，用0-1之间的浮点数将矩阵变换为可视的图像格式
	cv::normalize(magImage, magImage, 0, 1, cv::NORM_MINMAX);

	//cv::imshow("test2", magImage);
	magImage.convertTo(magImage, CV_8UC1, 255, 0);
	return magImage;
}



cv::Mat Utils::getPhaseImage(const cv::Mat& fourierImage) {
	//获得相位谱
	cv::Mat planes[] = { cv::Mat::zeros(fourierImage.size(), CV_32FC1),
						cv::Mat::zeros(fourierImage.size(), CV_32FC1) };

	cv::Mat phaseImage = planes[0].clone();
	cv::split(fourierImage, planes);
	cv::phase(planes[0], planes[1], phaseImage);
	phaseImage = changeCenter(phaseImage);
	//归一化处理，用0-1之间的浮点数将矩阵变换为可视的图像格式
	cv::normalize(phaseImage, phaseImage, 0, 1, cv::NORM_MINMAX); //归一化 方便显示，和实际数据没有关系

	phaseImage.convertTo(phaseImage, CV_8UC1, 255, 0);
	return phaseImage;
}

cv::Mat Utils::inverseFourierTransform(const cv::Mat& fourierImage) {
	cv::Mat dealtImage = fourierImage.clone();
	cv::Mat iDFT[] = { cv::Mat::zeros(dealtImage.size(), CV_32FC1),
					  cv::Mat::zeros(dealtImage.size(), CV_32FC1) };

	cv::idft(dealtImage, dealtImage);
	cv::split(dealtImage, iDFT);
	normalize(iDFT[0], iDFT[0], 0, 1, cv::NORM_MINMAX);
	iDFT[0].convertTo(iDFT[0], CV_8UC1, 255, 0);
	return iDFT[0];
}
