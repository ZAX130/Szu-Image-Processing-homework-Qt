#ifndef _RGB2HSV_
#define _RGB2HSV_

#pragma once

#include <QtWidgets/QWidget>
#include <QFileDialog>
#include <QImage>
#include <QGraphicsScene>
#include <QFileDialog>
#include "ui_RGB2HSV.h"
#include <QGraphicsView>
#include <Qcolor>
#include "Utils.h"
#include "PanelWin.h"
#include <qdebug.h>

class RGB2HSV : public QWidget
{
    Q_OBJECT

public:
    RGB2HSV(QWidget* parent = Q_NULLPTR);
private slots:
    void btn_chooseImg();
    void updateImage();
    void reloadImage();
private:
    Ui::RGB2HSVClass ui;
    QImage* image;
    QGraphicsScene* scene;
    PanelWin* panelwin;
    QString fileName;
};

#endif // _RGB2HSV_


