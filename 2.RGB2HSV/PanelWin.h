#ifndef _PanelWin_
#define _PanelWin_

#pragma once

#include <QWidget>
#include "ui_PanelWin.h"


class PanelWin : public QWidget
{
	Q_OBJECT
signals:
	void sliderChange();
	void resetScene();
private slots:
	void emitUpdateSignal();
	void resetSlider();
public:
	PanelWin(QWidget* parent = Q_NULLPTR);
	~PanelWin();
	Ui::PanelWin ui;
};

#endif // !_PanelWin_



