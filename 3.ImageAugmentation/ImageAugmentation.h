#pragma once

#include <QtWidgets/QWidget>
#include "ui_ImageAugmentation.h"
#include <QtWidgets/QWidget>
#include <QFileDialog>
#include <QImage>
#include <QGraphicsScene>
#include <QFileDialog>
#include <algorithm>
#include <QGraphicsView>
#include <Qcolor>
#include "Utils.h"
#include "PanelWin.h"
#include <qdebug.h>
#include <QtCharts/QtCharts>
#include <QtCharts/QBarSet>
#include <QtCharts/QLineSeries>
QT_CHARTS_USE_NAMESPACE
using namespace std;
class ImageAugmentation : public QWidget
{
    Q_OBJECT

public:
    explicit ImageAugmentation(QWidget *parent = Q_NULLPTR);
    

private slots:
    void btn_chooseImg();
    void TransImage();
    void reloadImage();
    void drawHist();
    void histReg();
    void histUniform();
    void meanfilter();
    void gaussfilter();
    void sobelfilter();
    void laplasfilter();
    void medianfilter();
    void maxfilter();
    void fcolor();
    void minfilter();
private:
    Ui::ImageAugmentationClass ui;
    QImage* image;
    QGraphicsScene* scene;
    PanelWin* panelwin;
    QString fileName;
    int grayPixelNum[256];
};
