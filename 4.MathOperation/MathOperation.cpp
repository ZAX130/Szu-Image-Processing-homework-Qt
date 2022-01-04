#include "MathOperation.h"


MathOperation::MathOperation(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
	image1 = new QImage;
	scene1 = new QGraphicsScene;
	image2 = new QImage;
	scene2 = new QGraphicsScene;
	scene3 = new QGraphicsScene;
	fileName1 = "";
	fileName2 = "";
	ui.warning->setVisible(false);
	ui.graphicsView_1->setScene(scene1);
	ui.graphicsView_2->setScene(scene2);
	ui.graphicsView_3->setScene(scene3);


    connect(ui.btn_image1, &QPushButton::clicked, this, &MathOperation::btn_chooseImg1);
	connect(ui.btn_image2, &QPushButton::clicked, this, &MathOperation::btn_chooseImg2);
	connect(ui.plus_rb,&QRadioButton::clicked, this, &MathOperation::plusOP);
	connect(ui.minus_rb , &QRadioButton::clicked, this, & MathOperation::minusOP);
	connect(ui.multiply_rb, &QRadioButton::clicked, this, & MathOperation::multiOP);
	connect(ui.divide_rb, &QRadioButton::clicked, this, & MathOperation::divideOP);
	connect(ui.and_rb, &QRadioButton::clicked, this, & MathOperation::andOP);
	connect(ui.or_rb, &QRadioButton::clicked, this, & MathOperation::orOP);
	connect(ui.not_rb, &QRadioButton::clicked, this, & MathOperation::notOP);
	connect(ui.xor_rb, &QRadioButton::clicked, this, & MathOperation::xorOP);
	connect(ui.horiflip_rb, &QRadioButton::clicked, this, & MathOperation::horiFilpOP);
	connect(ui.verflip_rb, &QRadioButton::clicked, this, & MathOperation::vertiFilpOP);
	connect(ui.scale_rb, &QRadioButton::clicked, this, & MathOperation::ScaleOP);
	connect(ui.plus_slider, &QSlider::valueChanged,this, & MathOperation::addSilderChange);
	connect(ui.plus_slider, &QSlider::sliderReleased, this, & MathOperation::addSilderReleased);
	connect(ui.divide_slider, &QSlider::valueChanged, this, & MathOperation::divideSilderChange);
	connect(ui.divide_slider, &QSlider::sliderReleased,this, & MathOperation::divideSilderReleased);
	connect(ui.scale_slider, &QSlider::valueChanged, this, & MathOperation::scaleSilderChange);
	connect(ui.scale_slider, &QSlider::sliderReleased,this, & MathOperation::scaleSilderReleased);
	connect(ui.Reset_btn, &QPushButton::clicked, this, &MathOperation::reset);
}

void MathOperation::btn_chooseImg1() {
	//打开图片
	fileName1 = QFileDialog::getOpenFileName(this,
		tr("Open Input Image1"),
		QDir::currentPath(),
		tr("image") + "(*.jpg *.png *bmp)");
	if (fileName1 != "" && image1->load(fileName1))
	{
		QSize size(image1->width(), image1->height());
		image3 = new QImage(size, QImage::Format_RGB32);
		ui.warning->setVisible(false);
		scene1->clear();
		scene1->addPixmap(QPixmap::fromImage(*image1));
		if (fileName2 != "") {
			*image2 = image2->scaled(size, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
			scene2->clear();
			scene2->addPixmap(QPixmap::fromImage(*image2));
		}
		reset();
	}
}

void MathOperation::btn_chooseImg2()
{
	//打开图片
	if (fileName1 == "") {
		ui.warning->setVisible(true);
		return;
	}
	fileName2 = QFileDialog::getOpenFileName(this,
		tr("Open Input Image2"),
		QDir::currentPath(),
		tr("image") + "(*.jpg *.png *bmp)");
	if (fileName2 != "" && image2->load(fileName2))
	{
		*image2 = image2->scaled(image1->width(), image1->height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
		scene2->clear();
		scene2->addPixmap(QPixmap::fromImage(*image2));
		ui.graphicsView_2->setScene(scene2);
		reset();
	}
}

void MathOperation::outputImg3()
{
	scene3->addPixmap(QPixmap::fromImage(*image3));
	ui.graphicsView_3->setScene(scene3);
}

void MathOperation::reset()
{
	scene3->clear();
	ui.graphicsView_2->setScene(scene2);
	ui.plus_slider->setValue(0);
	ui.alpha->setText("0");
	ui.beta->setText("1");
	ui.divide_slider->setValue(0);
	ui.Constant->setText("1");
	ui.scale_slider->setValue(0);
	ui.times->setText("1");


	ui.plus_rb->setCheckable(true);
	ui.minus_rb->setCheckable(true);
	ui.multiply_rb->setCheckable(true);
	ui.divide_rb->setCheckable(true);
	ui.and_rb->setCheckable(true);
	ui.or_rb->setCheckable(true);
	ui.not_rb->setCheckable(true);
	ui.xor_rb->setCheckable(true);
	ui.horiflip_rb->setCheckable(true);
	ui.verflip_rb->setCheckable(true);
	ui.scale_rb->setCheckable(true);

	ui.op->setText("op");
}

void MathOperation::plusOP()
{
	ui.op->setText("+");
	double a =  ui.alpha->text().toDouble();
	double b = ui.beta->text().toDouble();
	for (int i = 0;i < image1->width();i++) {
		for (int j = 0;j < image1->height();j++) {
			int r1 = image1->pixelColor(i, j).red();
			int g1 = image1->pixelColor(i, j).green();
			int b1 = image1->pixelColor(i, j).blue();
			int r2 = image2->pixelColor(i, j).red();
			int g2 = image2->pixelColor(i, j).green();
			int b2 = image2->pixelColor(i, j).blue();
			int r3 = Utils::plus(a, r1, b, r2);
			int g3 = Utils::plus(a, g1, b, g2);
			int b3 = Utils::plus(a, b1, b, b2);
			image3->setPixel(i, j, qRgb(r3, g3, b3));
		}
	}
	scene3->clear();
	scene3->addPixmap(QPixmap::fromImage(*image3));
}

void MathOperation::minusOP()
{
	ui.op->setText("-");
	for (int i = 0;i < image1->width();i++) {
		for (int j = 0;j < image1->height();j++) {
			int r1 = image1->pixelColor(i, j).red();
			int g1 = image1->pixelColor(i, j).green();
			int b1 = image1->pixelColor(i, j).blue();
			int r2 = image2->pixelColor(i, j).red();
			int g2 = image2->pixelColor(i, j).green();
			int b2 = image2->pixelColor(i, j).blue();
			int r3 = Utils::minus(r1,r2);
			int g3 = Utils::minus(g1,g2);
			int b3 = Utils::minus(b1, b2);
			image3->setPixel(i, j, qRgb(r3, g3, b3));
		}
	}
	scene3->clear();
	scene3->addPixmap(QPixmap::fromImage(*image3));
}

void MathOperation::multiOP()
{
	ui.op->setText("x");
	for (int i = 0;i < image1->width();i++) {
		for (int j = 0;j < image1->height();j++) {
			int r1 = image1->pixelColor(i, j).red();
			int g1 = image1->pixelColor(i, j).green();
			int b1 = image1->pixelColor(i, j).blue();
			int r2 = image2->pixelColor(i, j).red();
			int g2 = image2->pixelColor(i, j).green();
			int b2 = image2->pixelColor(i, j).blue();
			int r3 = Utils::multiply(r1, r2);
			int g3 = Utils::multiply(g1, g2);
			int b3 = Utils::multiply(b1, b2);
			image3->setPixel(i, j, qRgb(r3, g3, b3));
		}
	}
	scene3->clear();
	scene3->addPixmap(QPixmap::fromImage(*image3));
}

void MathOperation::divideOP()
{
	ui.op->setText("/");
	double C = ui.Constant->text().toDouble();
	for (int i = 0;i < image1->width();i++) {
		for (int j = 0;j < image1->height();j++) {
			int r1 = image1->pixelColor(i, j).red();
			int g1 = image1->pixelColor(i, j).green();
			int b1 = image1->pixelColor(i, j).blue();
			int r3 = Utils::divide(r1, C);
			int g3 = Utils::divide(g1, C);
			int b3 = Utils::divide(b1, C);
			image3->setPixel(i, j, qRgb(r3, g3, b3));
		}
	}
	scene3->clear();
	scene3->addPixmap(QPixmap::fromImage(*image3));
}

void MathOperation::andOP()
{
	ui.op->setText("&");
	for (int i = 0;i < image1->width();i++) {
		for (int j = 0;j < image1->height();j++) {
			int r1 = image1->pixelColor(i, j).red();
			int g1 = image1->pixelColor(i, j).green();
			int b1 = image1->pixelColor(i, j).blue();
			int r2 = image2->pixelColor(i, j).red();
			int g2 = image2->pixelColor(i, j).green();
			int b2 = image2->pixelColor(i, j).blue();
			int r3 = Utils::andOp(r1, r2);
			int g3 = Utils::andOp(g1, g2);
			int b3 = Utils::andOp(b1, b2);
			image3->setPixel(i, j, qRgb(r3, g3, b3));
		}
	}
	scene3->clear();
	scene3->addPixmap(QPixmap::fromImage(*image3));
}

void MathOperation::orOP()
{
	ui.op->setText("|");
	for (int i = 0;i < image1->width();i++) {
		for (int j = 0;j < image1->height();j++) {
			int r1 = image1->pixelColor(i, j).red();
			int g1 = image1->pixelColor(i, j).green();
			int b1 = image1->pixelColor(i, j).blue();
			int r2 = image2->pixelColor(i, j).red();
			int g2 = image2->pixelColor(i, j).green();
			int b2 = image2->pixelColor(i, j).blue();
			int r3 = Utils::orOp(r1, r2);
			int g3 = Utils::orOp(g1, g2);
			int b3 = Utils::orOp(b1, b2);
			image3->setPixel(i, j, qRgb(r3, g3, b3));
		}
	}
	scene3->clear();
	scene3->addPixmap(QPixmap::fromImage(*image3));

}

void MathOperation::notOP()
{
	ui.op->setText("!");
	for (int i = 0;i < image1->width();i++) {
		for (int j = 0;j < image1->height();j++) {
			int r1 = image1->pixelColor(i, j).red();
			int g1 = image1->pixelColor(i, j).green();
			int b1 = image1->pixelColor(i, j).blue();
			int r2 = image2->pixelColor(i, j).red();
			int g2 = image2->pixelColor(i, j).green();
			int b2 = image2->pixelColor(i, j).blue();
			int r3 = Utils::notOp(r1);
			int g3 = Utils::notOp(g1);
			int b3 = Utils::notOp(b1);
			image3->setPixel(i, j, qRgb(r3, g3, b3));
		}
	}
	scene3->clear();
	scene3->addPixmap(QPixmap::fromImage(*image3));

}

void MathOperation::xorOP()
{
	ui.op->setText("~");
	for (int i = 0;i < image1->width();i++) {
		for (int j = 0;j < image1->height();j++) {
			int r1 = image1->pixelColor(i, j).red();
			int g1 = image1->pixelColor(i, j).green();
			int b1 = image1->pixelColor(i, j).blue();
			int r2 = image2->pixelColor(i, j).red();
			int g2 = image2->pixelColor(i, j).green();
			int b2 = image2->pixelColor(i, j).blue();
			int r3 = Utils::xorOp(r1,r2);
			int g3 = Utils::xorOp(g1,g2);
			int b3 = Utils::xorOp(b1,b2);
			image3->setPixel(i, j, qRgb(r3, g3, b3));
		}
	}
	scene3->clear();
	scene3->addPixmap(QPixmap::fromImage(*image3));
}

void MathOperation::horiFilpOP()
{
	for (int i = 0;i < image1->width();i++) {
		for (int j = 0;j < image1->height();j++) {
			image3->setPixel(image1->width()-i, j, image1->pixelColor(i, j).rgb());
		}
	}
	scene3->clear();
	scene3->addPixmap(QPixmap::fromImage(*image3));
}

void MathOperation::vertiFilpOP()
{
	for (int i = 0;i < image1->width();i++) {
		for (int j = 0;j < image1->height();j++) {
			image3->setPixel(i, image1->height()-j, image1->pixelColor(i, j).rgb());
		}
	}
	scene3->clear();
	scene3->addPixmap(QPixmap::fromImage(*image3));
}

void MathOperation::ScaleOP()
{
	double times = ui.times->text().toDouble();
	*image3 = image1->scaled((int)image1->width()*times, (int)image1->height()*times, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	scene3->clear();
	scene3->addPixmap(QPixmap::fromImage(*image3));
}

void MathOperation::addSilderChange()
{
	if (ui.plus_rb->isChecked()) {
		int a = ui.plus_slider->value();
		ui.alpha->setText(QString::number(1 - a / 100.0));
		ui.beta->setText(QString::number(a / 100.0));
		plusOP();
	}
}

void MathOperation::addSilderReleased()
{
	addSilderChange();
}

void MathOperation::divideSilderChange()
{
	if (ui.divide_rb->isChecked()) {
		int a = ui.divide_slider->value();
		if (a > 0) {
			ui.Constant->setText(QString::number(a/100.0+1));
		}
		else {
			ui.Constant->setText(QString::number(exp(0.01*a)));
		}
		divideOP();
	}
}

void MathOperation::divideSilderReleased()
{
	divideSilderChange();
}

void MathOperation::scaleSilderChange()
{
	if (ui.scale_rb->isChecked()) {
		int a = ui.scale_slider->value();
		ui.times->setText(QString::number(pow(2,a)));
		ScaleOP();
	}
}

void MathOperation::scaleSilderReleased()
{
	scaleSilderChange();
}
