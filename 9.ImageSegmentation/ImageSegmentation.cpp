#include "ImageSegmentation.h"

ImageSegmentation::ImageSegmentation(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	fileName = "";
	isGrowing = false;
	scene1 = new QGraphicsScene;
	scene2 = new QGraphicsScene;

	ui.graphicsView->setScene(scene1);
	ui.graphicsView_2->setScene(scene2);
	ui.label_warning->setVisible(false);
	connect(ui.btn_chooseImg, &QPushButton::clicked, this, &ImageSegmentation::btn_chooseImg);
	connect(ui.btn_hand, &QPushButton::clicked, this, &ImageSegmentation::btn_ManiSplitImg);
	connect(ui.btn_otus, &QPushButton::clicked, this, &ImageSegmentation::btn_OtusSplitImg);
	connect(ui.btn_regionGrow, &QPushButton::clicked, this, &ImageSegmentation::btn_RegionGrow);
	connect(ui.btn_regionSplit, &QPushButton::clicked, this, &ImageSegmentation::btn_RegionSplit);
	connect(ui.graphicsView, &QMyGraphicsView::updateItemPos, this, &ImageSegmentation::updateItemPos);
}

QImage ImageSegmentation::cvMat2QImage(const cv::Mat& mat)
{
	QImage img;
	img = QImage((const unsigned char*)(mat.data),
		mat.cols, mat.rows,
		mat.cols * mat.channels(),
		QImage::Format_Grayscale8);

	return img;
}
Mat ImageSegmentation::QImage2cvMat(QImage image)
{
	Mat mat;
	qDebug() << image.format();
	switch (image.format())
	{
	case QImage::Format_ARGB32:
	case QImage::Format_RGB32:
	case QImage::Format_ARGB32_Premultiplied:
		mat = Mat(image.height(), image.width(), CV_8UC4, (void*)image.constBits(), image.bytesPerLine());
		break;
	case QImage::Format_RGB888:
		mat = Mat(image.height(), image.width(), CV_8UC3, (void*)image.constBits(), image.bytesPerLine());
		cvtColor(mat, mat, COLOR_BGR2RGB);
		break;
	case QImage::Format_Indexed8:
		mat = Mat(image.height(), image.width(), CV_8UC1, (void*)image.constBits(), image.bytesPerLine());
		break;
	}
	return mat;
}
void ImageSegmentation::btn_chooseImg()
{
	//打开图片
	fileName = QFileDialog::getOpenFileName(this,
		tr("Open Input Image"),
		QDir::currentPath(),
		tr("image") + "(*.jpg *.png *bmp)");
	if (fileName != "")
	{
		image1 = imread(fileName.toStdString(), IMREAD_GRAYSCALE);
		qDebug() << image1.rows << image1.cols;
		if (!image1.empty())
		{
			isGrowing = false;
			ui.label_warning->setVisible(false);
			scene1->clear();
			qimage1 = cvMat2QImage(image1);
			qDebug() << qimage1.width() << qimage1.height();
			scene1->addPixmap(QPixmap::fromImage(qimage1));
			tmpGrowingMask = Mat::zeros(image1.size(), CV_8UC1);
		}
	}
}

void ImageSegmentation::btn_ManiSplitImg()
{
	isGrowing = false;
	ui.label_warning->setVisible(false);
	tmpGrowingMask = Mat::zeros(image1.size(), CV_8UC1);
	scene2->clear();
	int v = ui.spin_threshold->value();
	threshold(image1, image2, v, 255, THRESH_BINARY);
	qimage2 = cvMat2QImage(image2);
	scene2->addPixmap(QPixmap::fromImage(qimage2));
}

void ImageSegmentation::btn_OtusSplitImg()
{
	isGrowing = false;
	ui.label_warning->setVisible(false);
	tmpGrowingMask = Mat::zeros(image1.size(), CV_8UC1);
	scene2->clear();
	threshold(image1, image2, 0, 255, THRESH_OTSU);
	qimage2 = cvMat2QImage(image2);
	scene2->addPixmap(QPixmap::fromImage(qimage2));
}

void ImageSegmentation::btn_RegionGrow()
{
	isGrowing = true;
	ui.label_warning->setVisible(true);
	tmpGrowingMask = Mat::zeros(image1.size(), CV_8UC1);
	qimage2 = cvMat2QImage(tmpGrowingMask);
	scene2->addPixmap(QPixmap::fromImage(qimage2));
}

void ImageSegmentation::btn_RegionSplit()
{
	isGrowing = false;
	ui.label_warning->setVisible(false);
	tmpGrowingMask = Mat::zeros(image1.size(), CV_8UC1);
	scene2->clear();
	RegionCut(image1, image2);
	qimage2 = cvMat2QImage(image2);
	scene2->addPixmap(QPixmap::fromImage(qimage2));
}

void ImageSegmentation::updateItemPos(QPointF pos)
{
	//根据获得的坐标,给出RGB像素值
	int x = pos.x();
	int y = pos.y();
	int th = ui.spin_growthreshold->value();
	QString str = "(" + QString::number(x) + "," + QString::number(y) + ")";
	ui.label_coordinate->setText(str);
	scene2->clear();
	image2 = RegionGrow(image1, Point2i(x, y), th);
	bitwise_or(image2, tmpGrowingMask, image2);    //和前一个分割的图做或运算
	tmpGrowingMask = image2.clone();    //保存前一个分割图
	//image2 = tmpGrowingMask + image2;
	for (int i = 0;i < image2.rows;i++) {
		for (int j = 0;j < image2.cols;j++) {
			if (image2.at<uchar>(i, j) != 0)
				image2.at<uchar>(i, j) = 255;
		}
	}
	qimage2 = cvMat2QImage(image2);
	scene2->addPixmap(QPixmap::fromImage(qimage2));
}
void ImageSegmentation::RegionCut(Mat src, Mat& src1) {
	struct SplitStruct {
		int width;  //图像的宽度
		int height;  //图像的高度
		int x;    //相对原图像数据的偏移宽度
		int y;    //相对原图像数据的偏移高度
	};
	stack<SplitStruct> splitPt;
	SplitStruct ss, ssTemp;
	ss.width = src.rows;
	ss.height = src.cols;
	ss.x = 0;
	ss.y = 0;
	splitPt.push(ss);
	src.copyTo(src1);
	int values[2][2];//存储每次分类后的区间
	double sn[2][2];
	int Height[3], Width[3];//存储每次分裂后没个块的大小

	while (!splitPt.empty()) {
		ss = splitPt.top();//将栈顶元素幅值给结构体
		splitPt.pop();   //栈顶元素出栈

		Height[0] = 0;
		Height[1] = (ss.height + 1) / 2;//第一个块的高
		Height[2] = ss.height - Height[1];//第二个块的高

		Width[0] = 0;
		Width[1] = (ss.width + 1) / 2;  //第一个块的宽
		Width[2] = ss.width - Width[1];//第二个块的宽

		for (int i = 1;i < 3;i++) {
			for (int j = 1;j < 3;j++) {
				//计算每一个区域的平均灰度值
				int Sumgray = 0;
				for (int p = 0;p < Width[i];p++) {
					for (int q = 0;q < Height[j];q++) {
						Sumgray += src.at<uchar>(p + ss.x, q + ss.y);
					}
				}
				if (Width[i] * Height[j] == 0)
					continue;
				if (Width[i] * Height[j] == 1) {    //即分到最小像素
					if (src.at<uchar>(Width[i - 1] + ss.x, Height[j - 1] + ss.y) < 100) {
						src1.at<uchar>(Width[i - 1] + ss.x, Height[j - 1] + ss.y) = 0;
					}
					else {
						src1.at<uchar>(Width[i - 1] + ss.x, Height[j - 1] + ss.y) = 255;
					}
					continue;
				}
				values[i - 1][j - 1] = Sumgray / (Height[j] * Width[i]);

				//判断是否需要继续分裂
				double sngray = 0.0;
				for (int p = 0;p < Width[i];p++) {
					for (int q = 0;q < Height[j];q++) {
						//计算方差
						sngray += (src.at<uchar>(p + Width[i - 1] + ss.x, q + Height[j - 1] + ss.y) - values[i - 1][j - 1]) * (src.at<uchar>(p + Width[i - 1] + ss.x, q + Height[j - 1] + ss.y) - values[i - 1][j - 1]);
					}
				}
				sn[i - 1][j - 1] = sngray / (Height[j] * Width[i]);

				//判断是否需要继续分裂
				if (sn[i - 1][j - 1] > 16) {   //如果该区域内的灰度值不满足设定阈值，则继续分裂，否则合并
					//设置新的区域的大小以及相对原点像素点偏移的位置
					ssTemp.width = Width[i];
					ssTemp.height = Height[j];
					ssTemp.x = ss.x + Width[i - 1];
					ssTemp.y = ss.y + Height[j - 1];
					splitPt.push(ssTemp);          //将新的结构体元素入栈
				}
				else {     //如果不继续分裂则合并
					for (int xx = 0;xx < Width[i];xx++) {
						for (int yy = 0;yy < Height[j];yy++) {
							src1.at<uchar>(xx + Width[i - 1] + ss.x, yy + Height[j - 1] + ss.y) = 0;   //即把该区域内的所有像素在src1中以0的灰度绘制出来
						}
					}
				}
			}
		}
	}
}

Mat ImageSegmentation::RegionGrow(Mat src, Point2i pt, int th)
{
	Point2i ptGrowing;						//待生长点位置
	int nGrowLable = 0;								//标记是否生长过
	int nSrcValue = 0;								//生长起点灰度值
	int nCurValue = 0;								//当前生长点灰度值
	Mat matDst = Mat::zeros(src.size(), CV_8UC1);	//创建一个空白区域，填充为黑色
	//生长方向顺序数据
	int DIR[8][2] = { { -1, -1 }, { 0, -1 }, { 1, -1 }, { 1, 0 }, { 1, 1 }, { 0, 1 }, { -1, 1 }, { -1, 0 } };
	vector<Point2i> vcGrowPt;						//生长点栈
	vcGrowPt.push_back(pt);							//将生长点压入栈中
	matDst.at<uchar>(pt.y, pt.x) = 255;				//标记生长点
	nSrcValue = src.at<uchar>(pt.y, pt.x);			//记录生长点的灰度值

	while (!vcGrowPt.empty())						//生长栈不为空则生长
	{
		pt = vcGrowPt.back();						//取出一个生长点
		vcGrowPt.pop_back();

		//分别对八个方向上的点进行生长
		for (int i = 0; i < 9; ++i)
		{
			ptGrowing.x = pt.x + DIR[i][0];
			ptGrowing.y = pt.y + DIR[i][1];
			//检查是否是边缘点
			if (ptGrowing.x < 0 || ptGrowing.y < 0 || ptGrowing.x >(src.cols - 1) || (ptGrowing.y > src.rows - 1))
				continue;

			nGrowLable = matDst.at<uchar>(ptGrowing.y, ptGrowing.x);		//当前待生长点的灰度值

			if (nGrowLable == 0)					//如果标记点还没有被生长
			{
				nCurValue = src.at<uchar>(ptGrowing.y, ptGrowing.x);
				if (abs(nSrcValue - nCurValue) < th)					//在阈值范围内则生长
				{
					matDst.at<uchar>(ptGrowing.y, ptGrowing.x) = 255;		//标记为白色
					vcGrowPt.push_back(ptGrowing);					//将下一个生长点压入栈中
				}
			}
		}
	}
	return matDst.clone();
}