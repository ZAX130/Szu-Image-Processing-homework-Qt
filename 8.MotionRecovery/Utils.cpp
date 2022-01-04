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

cv::Mat Utils::LogfourierTransform(const cv::Mat& originImage) {
	cv::Mat paddingImage;

	//得到适合傅里叶变换的最优尺寸
	int m = cv::getOptimalDFTSize(originImage.rows);
	int n = cv::getOptimalDFTSize(originImage.cols);

	//填充，上侧和左侧不填充
	cv::copyMakeBorder(originImage, paddingImage, 0, m - originImage.rows,
		0, n - originImage.cols, cv::BORDER_CONSTANT, cv::Scalar(0));
	paddingImage.convertTo(paddingImage, CV_32FC1);
	cv::log(paddingImage + 1, paddingImage);
	//paddingImage = paddingImage * 255 / log(255);
	//idft有个bug,回来的数非常大,必须归一才能恢复原图像,不过这种是强制让原图到0~255,除非记录下原图像的最大最小值,再压缩回去
	//双通道Mat
	cv::Mat planes[] = { cv::Mat_<float>(paddingImage), cv::Mat::zeros(paddingImage.size(), CV_32FC1) };
	cv::Mat mergeImage;
	cv::merge(planes, 2, mergeImage);
	//cout << "-------------------merge--------------------------------------" << format(mergeImage(cv::Rect(50, 50, 3, 3)), cv::Formatter::FMT_PYTHON) << endl;
	//进行傅里叶变换
	cv::dft(mergeImage, mergeImage, cv::DFT_COMPLEX_OUTPUT);
	//cout << "-------------------merge--------------------------------------" << format(mergeImage(cv::Rect(50,50,3,3)), cv::Formatter::FMT_PYTHON) << endl;
	return mergeImage;
}
cv::Mat Utils::expinverseFourierTransform(const cv::Mat& fourierImage) {
	cv::Mat dealtImage = fourierImage.clone();
	//cout << "-------------------dealt--------------------------------------" << format(dealtImage(cv::Rect(50, 50, 3, 3)), cv::Formatter::FMT_PYTHON) << endl;
	cv::Mat iDFT[] = { cv::Mat::zeros(dealtImage.size(), CV_32FC1),
					  cv::Mat::zeros(dealtImage.size(), CV_32FC1) };

	cv::idft(dealtImage, dealtImage);
	//cout << "-------------------idft dealt--------------------------------------" << format(dealtImage(cv::Rect(50, 50, 3, 3)), cv::Formatter::FMT_PYTHON) << endl;
	cv::split(dealtImage, iDFT);
	//cv::exp(iDFT[0], iDFT[0]);

	normalize(iDFT[0], iDFT[0], 1, log(256), cv::NORM_MINMAX);
	cv::exp(iDFT[0], iDFT[0]);
	iDFT[0] -= 1;
	//cout << "-------------------idft dealt--------------------------------------" << format(iDFT[0](cv::Rect(50, 50, 3, 3)), cv::Formatter::FMT_PYTHON) << endl;
	iDFT[0].convertTo(iDFT[0], CV_8UC1, 1, 0);
	//cout << "-------------------idft dealt--------------------------------------" << format(iDFT[0](cv::Rect(50, 50, 3, 3)), cv::Formatter::FMT_PYTHON) << endl;
	return iDFT[0];
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

// 图像边界处理
cv::Mat Utils::image_make_border(cv::Mat& src)
{
	int w = cv::getOptimalDFTSize(src.cols); // 获取DFT变换的最佳宽度
	int h = cv::getOptimalDFTSize(src.rows); // 获取DFT变换的最佳高度

	cv::Mat padded;
	// 常量法扩充图像边界，常量 = 0
	cv::copyMakeBorder(src, padded, 0, h - src.rows, 0, w - src.cols, cv::BORDER_CONSTANT, cv::Scalar::all(0));
	padded.convertTo(padded, CV_32FC1);
	return padded;
}

cv::Mat Utils::changeCenter(const cv::Mat& magImage) {
	cv::Mat magImageCopy = magImage.clone();
	//如果有奇数行或列，则转换为偶数
	magImageCopy = magImage(cv::Rect(0, 0, magImage.cols - (magImage.cols % 2), magImage.rows - (magImage.rows % 2)));
	//重新排布傅里叶变换后的图像，使得原点位于图像中心
	int centerX = magImageCopy.cols / 2;
	int centerY = magImageCopy.rows / 2;

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

// 巴特沃斯低通滤波核函数
cv::Mat Utils::butterworth_low_kernel(cv::Mat& scr, float sigma, int n)
{
	cv::Mat butterworth_low_pass(scr.size(), CV_32FC1); //，CV_32FC1
	float D0 = sigma;//半径D0越小，模糊越大；半径D0越大，模糊越小
	for (int i = 0; i < scr.rows; i++) {
		for (int j = 0; j < scr.cols; j++) {
			float d = sqrt(pow(float(i - scr.rows / 2), 2) + pow(float(j - scr.cols / 2), 2));//分子,计算pow必须为float型
			butterworth_low_pass.at<float>(i, j) = 1.0f / (1.0f + pow(d / D0, 2 * n));
		}
	}
	return butterworth_low_pass;
}

// 巴特沃斯低通滤波
cv::Mat Utils::getButterworth_low_kernel(cv::Mat& src, float d0, int n)
{
	// H = 1 / (1+(D/D0)^2n)   n表示巴特沃斯滤波器的次数
	// 阶数n=1 无振铃和负值    阶数n=2 轻微振铃和负值  阶数n=5 明显振铃和负值   阶数n=20 与ILPF相似
	cv::Mat padded = image_make_border(src);
	cout << "函数pad的" << padded.size().height << padded.size().width;
	cv::Mat butterworth_kernel = butterworth_low_kernel(padded, d0, n);
	return butterworth_kernel;
}

// 巴特沃斯高通滤波核函数
cv::Mat Utils::butterworth_high_kernel(cv::Mat& scr, float sigma, int n)
{
	cv::Mat butterworth_high_pass(scr.size(), CV_32FC1); //，CV_32FC1
	float D0 = (float)sigma;  // 半径D0越小，模糊越大；半径D0越大，模糊越小
	for (int i = 0; i < scr.rows; i++) {
		for (int j = 0; j < scr.cols; j++) {
			float d = sqrt(pow(float(i - scr.rows / 2), 2) + pow(float(j - scr.cols / 2), 2));//分子,计算pow必须为float型
			butterworth_high_pass.at<float>(i, j) = 1.0f - 1.0f / (1.0f + pow(d / D0, 2 * n));
		}
	}
	return butterworth_high_pass;
}

// 巴特沃斯高通滤波
cv::Mat Utils::getButterworth_high_kernel(cv::Mat& src, float d0, int n)
{
	// H = 1 / (1+(D/D0)^2n)   n表示巴特沃斯滤波器的次数
	// 阶数n=1 无振铃和负值    阶数n=2 轻微振铃和负值  阶数n=5 明显振铃和负值   阶数n=20 与ILPF相似
	cv::Mat padded = image_make_border(src);
	cout << "函数pad的" << padded.size().height << padded.size().width;
	cv::Mat butterworth_kernel = butterworth_high_kernel(padded, d0, n);
	return butterworth_kernel;
}

// 理想低通滤波核函数
cv::Mat Utils::ideal_low_kernel(cv::Mat& scr, float sigma)
{
	cv::Mat ideal_low_pass(scr.size(), CV_32FC1); //，CV_32FC1
	float D0 = sigma;//半径D0越小，模糊越大；半径D0越大，模糊越小
	for (int i = 0; i < scr.rows; i++) {
		for (int j = 0; j < scr.cols; j++) {
			float d_square = pow(float(i - scr.rows / 2), 2) + pow(float(j - scr.cols / 2), 2);//分子,计算pow必须为float型
			ideal_low_pass.at<float>(i, j) = d_square <= D0 * D0 ? 1.0f : 0.0f;
		}
	}
	return ideal_low_pass;
}

// 理想低通滤波
cv::Mat Utils::getideal_low_kernel(cv::Mat& src, float d0)
{
	cv::Mat padded = image_make_border(src);
	cout << "函数pad的" << padded.size().height << padded.size().width;
	cv::Mat ideal_kernel = ideal_low_kernel(padded, d0);
	return ideal_kernel;
}

// 理想高通滤波核函数
cv::Mat Utils::ideal_high_kernel(cv::Mat& scr, float sigma)
{
	cv::Mat ideal_high_pass(scr.size(), CV_32FC1); //，CV_32FC1
	float D0 = sigma;//半径D0越小，模糊越大；半径D0越大，模糊越小
	for (int i = 0; i < scr.rows; i++) {
		for (int j = 0; j < scr.cols; j++) {
			float d_square = pow(float(i - scr.rows / 2), 2) + pow(float(j - scr.cols / 2), 2);
			ideal_high_pass.at<float>(i, j) = d_square > D0 * D0 ? 1.0f : 0.0f;
		}
	}
	return ideal_high_pass;
}

// 理想高通滤波
cv::Mat Utils::getideal_high_kernel(cv::Mat& src, float d0)
{
	cv::Mat padded = image_make_border(src);
	cout << "函数pad的" << padded.size().height << padded.size().width;
	cv::Mat ideal_kernel = ideal_high_kernel(padded, d0);
	return ideal_kernel;
}

// 同态滤波核函数
cv::Mat Utils::homo_kernel(cv::Mat& scr, float gammaH, float gammaL, float sigma, int n)
{
	cv::Mat homo(scr.size(), CV_32FC1); //，CV_32FC1
	float D0 = (float)sigma;  // 半径D0
	for (int i = 0; i < scr.rows; i++) {
		for (int j = 0; j < scr.cols; j++) {
			float d_square = pow(float(i - scr.rows / 2), 2) + pow(float(j - scr.cols / 2), 2);
			homo.at<float>(i, j) = (gammaH - gammaL) * (1.0f - exp(-n * d_square / (D0 * D0))) + gammaL;
		}
	}
	return homo;
}

// 同态滤波
cv::Mat Utils::gethomo_kernel(cv::Mat& src, float gammaH, float gammaL, float d0, int n)
{
	cv::Mat padded = image_make_border(src);
	cout << "函数pad的" << padded.size().height << padded.size().width;
	cv::Mat homo = homo_kernel(padded, gammaH, gammaL, d0, n);
	return homo;
}

cv::Mat Utils::filtered_fourier(cv::Mat& blur, const cv::Mat& fourierImage)
{
	cv::Mat dealtImage = fourierImage.clone();
	cv::Mat plane[] = { cv::Mat::zeros(dealtImage.size(), CV_32FC1),
					  cv::Mat::zeros(dealtImage.size(), CV_32FC1) };
	cv::split(fourierImage, plane);

	// 以下的操作是频域迁移
	plane[0] = changeCenter(plane[0]);
	plane[1] = changeCenter(plane[1]);
	// *****************滤波器函数与DFT结果的乘积****************
	blur = blur(cv::Rect(0, 0, blur.cols - (blur.cols % 2), blur.rows - (blur.rows % 2)));
	cv::Mat blur_r, blur_i, BLUR;
	cv::multiply(plane[0], blur, blur_r);  // 滤波（实部与滤波器模板对应元素相乘）
	cv::multiply(plane[1], blur, blur_i);  // 滤波（虚部与滤波器模板对应元素相乘）
	cv::Mat plane1[] = { blur_r, blur_i };

	// 再次搬移回来进行逆变换
	plane1[0] = changeCenter(plane1[0]);
	plane1[1] = changeCenter(plane1[1]);
	cv::merge(plane1, 2, BLUR); // 实部与虚部合并
	return BLUR;
}

cv::Mat Utils::ToShowFormat(cv::Mat& image)
{
	cv::Mat result = image.clone();
	normalize(result, result, 0, 1, cv::NORM_MINMAX);
	result.convertTo(result, CV_8UC1, 255, 0);
	return result;
}