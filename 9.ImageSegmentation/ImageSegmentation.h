#pragma once

#include <QtWidgets/QWidget>
#include "ui_ImageSegmentation.h"
#include <opencv2/opencv.hpp>
#include <QFileDialog>
#include <QImage>
#include <QGraphicsScene>
#include <QFileDialog>
#include <algorithm>
#include <QGraphicsView>
#include <Qcolor>
#include <qdebug.h>
#include<stack>
#include<vector>
#include "QMyGraphicsView.h"
using namespace std;
using namespace cv;
class ImageSegmentation : public QWidget
{
	Q_OBJECT

public:
	ImageSegmentation(QWidget* parent = Q_NULLPTR);
	QImage cvMat2QImage(const cv::Mat& mat);
	cv::Mat QImage2cvMat(QImage image);
	void RegionCut(Mat src, Mat& src1);
	Mat RegionGrow(Mat src, Point2i pt, int th);
private slots:
	void btn_chooseImg();
	void btn_ManiSplitImg();
	void btn_OtusSplitImg();
	void btn_RegionGrow();
	void btn_RegionSplit();
	void updateItemPos(QPointF pos);
private:
	Mat image1, image2;
	Mat tmpGrowingMask;
	QImage qimage1, qimage2;
	QGraphicsScene* scene1, * scene2;
	QString fileName;
	bool isGrowing;
	Ui::ImageSegmentationClass ui;
};
