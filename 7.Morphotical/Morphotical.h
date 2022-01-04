#pragma once

#include <QtWidgets/QWidget>
#include "ui_Morphotical.h"
#include <opencv2/opencv.hpp>
#include <QFileDialog>
#include <QImage>
#include <QGraphicsScene>
#include <QFileDialog>
#include <algorithm>
#include <QGraphicsView>
#include <Qcolor>
#include "Utils.h"
#include <qdebug.h>
using namespace cv;
class Morphotical : public QWidget
{
    Q_OBJECT

public:
    Morphotical(QWidget *parent = Q_NULLPTR);
    QImage cvMat2QImage(const cv::Mat& mat);
	void showOriginImage();
	void showChangedImage();
private slots:
    void btn_chooseImg();
    void reset();
	void erosion();
	void expand();
	void open();
	void close();
    void useRect();
    void useCross();
    void useEllipse();
private:
    Mat image1, image2;
    QImage qimage1, qimage2;
    Mat structureElement;
    QGraphicsScene* scene1, * scene2, * scene3, * scene4, *scene5;
    QString fileName;
    Ui::MorphoticalClass ui;
};
