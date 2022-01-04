#pragma once

#include <QWidget>
#include "ui_PanelWin.h"
#include<QButtonGroup>
#include <QtCharts/QtCharts>
#include <QtCharts/QBarSet>
#include <QtCharts/QLineSeries>
#include <QGraphicsView>
#include <algorithm>
#include<QSpinBox>
#include "Utils.h"
QT_CHARTS_USE_NAMESPACE

class PanelWin : public QWidget
{
	Q_OBJECT
signals:
	void TransAug();
	void Resets();
	void HistReg();
	void HistUni();
	void Meanfilter();
	void Gaussfilter();
	void Sobelfilter();
	void Laplasfilter();
	void Medianfilter();
	void Maxfilter();
	void Fcolor();
	void Minfilter();
public slots:
	void drawline();
	void negTrans();
	void powerTrans();
	void segTrans();
	void logTrans();
	void expTrans();
	void emitTransAugSignal();
	void emitTransResetSignal();
	void emitHistUniSignal();
	void emitHistRegSignal();
	void emitMeanSignal();
	void emitGaussSignal();
	void emitSobelSignal();
	void emitLaplasSignal();
	void emitMedianSignal();
	void emitMaxSignal();
	void emiFcolorSignal();
	void emitMinSignal();
public:
	PanelWin(QWidget *parent = Q_NULLPTR);
	~PanelWin();
	Ui::PanelWin ui;
	int T[256];
};
