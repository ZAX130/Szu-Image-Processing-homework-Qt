#pragma once

#include <QtWidgets/QWidget>
#include "ui_MathOperation.h"
#include <QFileDialog>
#include <QImage>
#include <QGraphicsScene>
#include <QFileDialog>
#include <QGraphicsView>
#include <Qcolor>
#include "Utils.h"
#include <qdebug.h>
#include<QSize>
#include<cmath>
class MathOperation : public QWidget
{
    Q_OBJECT

public:
    MathOperation(QWidget *parent = Q_NULLPTR);

private slots:
    void btn_chooseImg1();
    void btn_chooseImg2();
    void outputImg3();
    void reset();
    void plusOP();
    void minusOP();
    void multiOP();
    void divideOP();
    void andOP();
    void orOP();
    void notOP();
    void xorOP();
    void horiFilpOP();
    void vertiFilpOP();
    void ScaleOP();
    void addSilderChange();
    void addSilderReleased();
    void divideSilderChange();
    void divideSilderReleased();
    void scaleSilderChange();
    void scaleSilderReleased();
private:
    Ui::MathOperationClass ui;
    QImage* image1, *image2, *image3;
    QGraphicsScene* scene1, *scene2, *scene3;
    QString fileName1, fileName2;
};
