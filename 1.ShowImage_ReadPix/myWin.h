#pragma once
#include <QtWidgets/QWidget>
#include <QImage>
#include <QGraphicsScene>
#include <QFileDialog>
#include "ui_myWin.h"
#include "QMyGraphicsView.h"
#include <Qcolor>

class myWin : public QWidget
{
    Q_OBJECT

public:
    myWin(QWidget* parent = Q_NULLPTR);
    ~myWin();
private slots:
    void btn_chooseImg();
    void updateItemPos(QPointF pos);
private:
    Ui::myWinClass ui;
    QImage* image;
    QGraphicsScene* scene;
};

