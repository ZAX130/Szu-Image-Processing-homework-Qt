#include "PanelWin.h"




PanelWin::PanelWin(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	ui.label_warning->setVisible(false);

	//绘制函数曲线
	for (int i = 0;i < 256;i++) {
		T[i] = i;
	}
	drawline();

	connect(ui.rb_neg, &QRadioButton::clicked, this, &PanelWin::negTrans);
	connect(ui.rb_log, &QRadioButton::clicked, this, &PanelWin::logTrans);
	connect(ui.rb_power, &QRadioButton::clicked, this, &PanelWin::powerTrans);
	connect(ui.rb_seg, &QRadioButton::clicked, this, &PanelWin::segTrans);
	connect(ui.rb_exp, &QRadioButton::clicked, this, &PanelWin::expTrans);
	connect(ui.x1, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &PanelWin::segTrans);
	connect(ui.x2, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &PanelWin::segTrans);
	connect(ui.y1, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &PanelWin::segTrans);
	connect(ui.y2, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &PanelWin::segTrans);

	connect(ui.log_a, static_cast<void(QDoubleSpinBox::*)(double) > (&QDoubleSpinBox::valueChanged), this, &PanelWin::logTrans);
	connect(ui.log_b, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &PanelWin::logTrans);
	connect(ui.e_a, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &PanelWin::expTrans);
	connect(ui.e_c, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &PanelWin::expTrans);
	connect(ui.x_b, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &PanelWin::powerTrans);
	connect(ui.x_r, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &PanelWin::powerTrans);
	connect(ui.btn_TransAug, &QPushButton::clicked, this, &PanelWin::emitTransAugSignal);
	connect(ui.btn_reset, &QPushButton::clicked, this, &PanelWin::emitTransResetSignal);
	connect(ui.btn_histUniform, &QPushButton::clicked, this, &PanelWin::emitHistUniSignal);
	connect(ui.btn_histRegular, &QPushButton::clicked, this, &PanelWin::emitHistRegSignal);

	connect(ui.btn_fcolor, &QPushButton::clicked, this, &PanelWin::emiFcolorSignal);
	connect(ui.btn_gauss, &QPushButton::clicked, this, &PanelWin::emitGaussSignal);
	connect(ui.btn_max, &QPushButton::clicked, this, &PanelWin::emitMaxSignal);
	connect(ui.btn_mean, &QPushButton::clicked, this, &PanelWin::emitMeanSignal);
	connect(ui.btn_median, &QPushButton::clicked, this, &PanelWin::emitMedianSignal);
	connect(ui.btn_min, &QPushButton::clicked, this, &PanelWin::emitMinSignal);
	connect(ui.btn_sobel, &QPushButton::clicked, this, &PanelWin::emitSobelSignal);
	connect(ui.btn_la, &QPushButton::clicked, this, &PanelWin::emitLaplasSignal);
}
void PanelWin::negTrans()
{
	if (!ui.rb_neg->isChecked()) return;
	Utils::NegTrans(T);
	//for (int i = 0;i < 256;i++) {
	//	qDebug() << T[i];
	//}
	drawline();
}

void PanelWin::powerTrans()
{
	if (!ui.rb_power->isChecked()) return;
	Utils::PowerTrans(T, ui.x_r->value(), ui.x_b->value());
	drawline();
}

void PanelWin::segTrans()
{
	if (!ui.rb_seg->isChecked()) return;
	if (ui.x1->value() > ui.x2->value() || ui.y1->value() > ui.y2->value()) {
		ui.label_warning->setVisible(true);
	}
	else {
		ui.label_warning->setVisible(false);
		Utils::SegTrans(T, ui.x1->value(), ui.y1->value(), ui.x2->value(), ui.y2->value());
		drawline();
	}
}

void PanelWin::logTrans()
{
	//qDebug() << ui.rb_log->isChecked();
	if (!ui.rb_log->isChecked()) return;
	//qDebug() << ui.log_a->value() << ui.log_b->value();
	Utils::LogTrans(T,ui.log_a->value(), ui.log_b->value());
	drawline();
}

void PanelWin::expTrans()
{
	if (!ui.rb_exp->isChecked()) return;
	Utils::ExpTrans(T,ui.e_c->value(), ui.e_a->value());
	drawline();
}

void PanelWin::emitTransAugSignal()
{
	emit TransAug();
}

void PanelWin::emitTransResetSignal()
{
	for (int i = 0;i < 256;i++) {
		T[i] = i;
	}
	drawline();
	ui.rb_exp->setChecked(false);
	ui.rb_log->setChecked(false);
	ui.rb_seg->setChecked(false);
	ui.rb_power->setChecked(false);
	ui.rb_neg->setChecked(false);

	emit Resets();
}

void PanelWin::emitHistUniSignal()
{
	emit HistUni();
}

void PanelWin::emitHistRegSignal()
{
	emit HistReg();
}

void PanelWin::emitMeanSignal()
{
	emit Meanfilter();
}

void PanelWin::emiFcolorSignal()
{
	emit Fcolor();
}

void PanelWin::emitMinSignal()
{
	emit Minfilter();
}

void PanelWin::emitGaussSignal()
{
	emit Gaussfilter();
}

void PanelWin::emitSobelSignal()
{
	emit Sobelfilter();
}

void PanelWin::emitLaplasSignal()
{
	emit Laplasfilter();
}

void PanelWin::emitMedianSignal()
{
	emit Medianfilter();
}

void PanelWin::emitMaxSignal()
{
	emit Maxfilter();
}

void PanelWin::drawline()
{
	QLineSeries* series = new QLineSeries();
	series->setName("spline");
	for (int i = 0;i < 256;i++) {
		series->append(i, T[i]);
	}

	QChart* chart = new QChart();
	chart->legend()->hide();//隐藏图例
	chart->addSeries(series);//添加数据
	chart->setTitle("Transformation");//标题
	chart->createDefaultAxes();//坐标系
	//chart->setAnimationOptions(QChart::SeriesAnimations);
	chart->axes(Qt::Vertical).first()->setRange(0, 255);//坐标系范围

	ui.lineview->setChart(chart);//添加到显示widget
}
PanelWin::~PanelWin()
{

}

