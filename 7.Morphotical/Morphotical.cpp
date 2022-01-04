#include "Morphotical.h"

Morphotical::Morphotical(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	fileName = "";

	scene1 = new QGraphicsScene;
	scene2 = new QGraphicsScene;

	ui.graphicsView->setScene(scene1);
	ui.graphicsView_2->setScene(scene2);

	structureElement = getStructuringElement(MORPH_RECT, Size(3, 3));

	connect(ui.btn_chooseImg, &QPushButton::clicked, this, &Morphotical::btn_chooseImg);
	connect(ui.btn_reset, &QPushButton::clicked, this, &Morphotical::reset);
	connect(ui.btn_erosion, &QPushButton::clicked, this, &Morphotical::erosion);
	connect(ui.btn_expand, &QPushButton::clicked, this, &Morphotical::expand);
	connect(ui.btn_open, &QPushButton::clicked, this, &Morphotical::open);
	connect(ui.btn_close, &QPushButton::clicked, this, &Morphotical::close);
	connect(ui.rb_rect, &QPushButton::clicked, this, &Morphotical::useRect);
	connect(ui.rb_cross, &QPushButton::clicked, this, &Morphotical::useCross);
	connect(ui.rb_ellipse, &QPushButton::clicked, this, &Morphotical::useEllipse);
}

void Morphotical::btn_chooseImg() {
	//打开图片
	fileName = QFileDialog::getOpenFileName(this,
		tr("Open Input Image"),
		QDir::currentPath(),
		tr("image") + "(*.jpg *.png *bmp)");
	if (fileName != "")
	{
		image1 = imread(fileName.toStdString(), IMREAD_GRAYSCALE);
		if (!image1.empty())
		{
			threshold(image1, image1, 128, 255, THRESH_BINARY);
			showOriginImage();
			image2 = image1.clone();
		}
	}
}

QImage Morphotical::cvMat2QImage(const cv::Mat& mat)
{
	QImage img;
	img = QImage((const unsigned char*)(mat.data),
		mat.cols, mat.rows,
		mat.cols * mat.channels(),
		QImage::Format_Grayscale8);
	img = img.scaled(ui.graphicsView->width() - 2, ui.graphicsView->height() - 2);
	return img;
}

void Morphotical::showOriginImage()
{
	scene1->clear();
	qimage1 = cvMat2QImage(image1);
	scene1->addPixmap(QPixmap::fromImage(qimage1));
}

void Morphotical::showChangedImage()
{
	scene2->clear();
	qimage2 = cvMat2QImage(image2);
	scene2->addPixmap(QPixmap::fromImage(qimage2));
}

void Morphotical::reset()
{
	if (fileName == "") return;
	image1 = imread(fileName.toStdString(), IMREAD_GRAYSCALE);
	if (!image1.empty())
	{
		showOriginImage();
		scene2->clear();
		image2 = image1.clone();
	}
}

void Morphotical::erosion()
{
	if (image1.empty()) return;
	//只需要修改image2,然后调用showChangedImage
	erode(image2, image2, structureElement);
	showChangedImage();
}

void Morphotical::expand()
{
	if (image1.empty()) return;
	dilate(image2, image2, structureElement);
	showChangedImage();
}

void Morphotical::open()
{
	if (image1.empty()) return;
	morphologyEx(image2, image2, MORPH_OPEN, structureElement);
	showChangedImage();
}

void Morphotical::close()
{
	if (image1.empty()) return;
	morphologyEx(image2, image2, MORPH_CLOSE, structureElement);
	showChangedImage();
}

void Morphotical::useRect()
{
	int w = ui.sb_w->value();
	int h = ui.sb_h->value();

	structureElement = getStructuringElement(MORPH_RECT, Size(w, h));
}

void Morphotical::useCross()
{
	int w = ui.sb_w->value();
	int h = ui.sb_h->value();
	structureElement = getStructuringElement(MORPH_CROSS, Size(w, h));
}

void Morphotical::useEllipse()
{
	int w = ui.sb_w->value();
	int h = ui.sb_h->value();
	structureElement = getStructuringElement(MORPH_ELLIPSE, Size(w, h));
}