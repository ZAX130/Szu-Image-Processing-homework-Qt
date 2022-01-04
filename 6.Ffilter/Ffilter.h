#pragma once

#include <QtWidgets/QWidget>
#include "ui_Ffilter.h"
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
class Ffilter : public QWidget
{
    Q_OBJECT

public:
    Ffilter(QWidget *parent = Q_NULLPTR);
    QImage cvMat2QImage(const cv::Mat& mat);
    cv::Mat QImage2cvMat(QImage image);
    void showOriginImage();
    void showFFTMagnitudeImage();
    void showFFTPhaseImage();
    void showIFFT();
private slots:
    void btn_chooseImg();
    void butterworth_Low();
    void butterworth_high();
    void ideal_low();
    void ideal_high();
    void homofilter();
    void D0silderChange();
    void nsilderChange();
    void gammaHsilderChange();
    void gammaLsilderChange();
    void reset();
private:
    Mat image1, image2, image3, image4, image5;
    QImage qimage1, qimage2, qimage3, qimage4, qimage5;
    QGraphicsScene* scene1, * scene2, * scene3, * scene4, *scene5;
    QString fileName;
    Mat fourierTransformImage;
    Mat origin_fourierTransformImage;
    Ui::FfilterClass ui;
};
