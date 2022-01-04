#pragma once

#include <QtWidgets/QWidget>
#include "ui_FFT.h"
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

class FFT : public QWidget
{
    Q_OBJECT

public:
    FFT(QWidget *parent = Q_NULLPTR);
    QImage cvMat2QImage(const cv::Mat& mat);
    cv::Mat QImage2cvMat(QImage image);
    void showOriginImage();
    void showFFTMagnitudeImage();
    void showFFTPhaseImage();
    void showIFFT();
private slots:
    //void DisplayMat(cv::Mat image);

    void btn_chooseImg();
private:
    Mat image1, image2, image3, image4;
    QImage qimage1, qimage2, qimage3,qimage4;
    QGraphicsScene* scene1, *scene2, *scene3, *scene4;
    QString fileName;
    Ui::FFTClass ui;
    Mat fourierTransformImage;
};
