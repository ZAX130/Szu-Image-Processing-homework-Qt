#include "PanelWin.h"

//#include "RGB2HSV.h"
PanelWin::PanelWin(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	connect(ui.hSlider, &QSlider::sliderReleased, this, &PanelWin::emitUpdateSignal);
	connect(ui.sSlider, &QSlider::sliderReleased, this, &PanelWin::emitUpdateSignal);
	connect(ui.vSlider, &QSlider::sliderReleased, this, &PanelWin::emitUpdateSignal);
	connect(ui.btn_Reset, &QPushButton::clicked, this, &PanelWin::resetSlider);

}


void PanelWin::emitUpdateSignal() {
	emit sliderChange();
}

PanelWin::~PanelWin()
{
}


void PanelWin::resetSlider()
{
	ui.hSlider->setValue(0);
	ui.sSlider->setValue(0);
	ui.vSlider->setValue(0);
	emit resetScene();
}
