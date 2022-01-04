#include "ImageAugmentation.h"


ImageAugmentation::ImageAugmentation(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
	panelwin = new PanelWin();
	connect(ui.pushButton, &QPushButton::clicked, this, &ImageAugmentation::btn_chooseImg);
	connect(panelwin, &PanelWin::TransAug, this, &ImageAugmentation::TransImage);//
	connect(panelwin, &PanelWin::Resets, this, &ImageAugmentation::reloadImage);//
	connect(panelwin, &PanelWin::HistReg, this, &ImageAugmentation::histReg);//
	connect(panelwin, &PanelWin::HistUni, this, &ImageAugmentation::histUniform);//

	connect(panelwin, &PanelWin::Meanfilter, this, &ImageAugmentation::meanfilter);//
	connect(panelwin, &PanelWin::Gaussfilter, this, &ImageAugmentation::gaussfilter);//
	connect(panelwin, &PanelWin::Sobelfilter, this, &ImageAugmentation::sobelfilter);//
	connect(panelwin, &PanelWin::Medianfilter, this, &ImageAugmentation::medianfilter);//
	connect(panelwin, &PanelWin::Maxfilter, this, &ImageAugmentation::maxfilter);//
	connect(panelwin, &PanelWin::Laplasfilter, this, &ImageAugmentation::laplasfilter);//
	connect(panelwin, &PanelWin::Fcolor, this, &ImageAugmentation::fcolor);//
	connect(panelwin, &PanelWin::Minfilter, this, &ImageAugmentation::minfilter);//
}


void ImageAugmentation::btn_chooseImg() {
	//打开图片
	fileName = QFileDialog::getOpenFileName(this,
		tr("Open Input Image"),
		QDir::currentPath(),
		tr("image") + "(*.jpg *.png *bmp)");
	if (fileName != "")
	{
		image = new QImage;
		if (image->load(fileName))
		{
			//重新布局控件
			// 显示图像
			scene = new QGraphicsScene;
			scene->addPixmap(QPixmap::fromImage(*image));
			ui.graphicsView->setScene(scene);
			ui.graphicsView->setFixedSize(image->width() + 2, image->height() + 2);
			ui.graphicsView->move(5, 5);

			//窗口大小调整
			setFixedSize(image->width() + 12, image->height() + 45);

			//按钮位置调整
			ui.pushButton->setFixedSize(image->width() + 2, 30);
			ui.pushButton->move(5, image->height() + 10);

			panelwin->show();

			drawHist();
		}
	}
}

void ImageAugmentation::drawHist()
{
	memset(grayPixelNum, 0, sizeof(grayPixelNum));
	for (int i = 0;i < image->width(); i++) {
		for (int j = 0;j < image->height();j++) {

			QColor color = QColor(image->pixel(i, j));
			grayPixelNum[color.red()]++;

		}
	}
	int yRange = 0;
	QBarSet* set = new QBarSet("灰度值");
	for (int i = 0;i < 256;i++) {
		set->append(grayPixelNum[i]);
		if (yRange < grayPixelNum[i]) yRange = grayPixelNum[i];
		set->setColor(QColor::Rgb);
	}
	QBarSeries* series = new QBarSeries();
	series->append(set);
	series->setBarWidth(0.5);

	//设置横坐标
	QStringList categories;
	for (int i = 0; i < 256; i++) {
		//qDebug() << grayPixelNum[i];
		categories.push_back("");
	}

	QBarCategoryAxis* axisX = new QBarCategoryAxis();
	axisX->append(categories);

	//设置纵坐标
	QValueAxis* axisY = new QValueAxis;
	axisY->setLabelFormat("%d");
	axisY->setRange(0, yRange);

	//建表
	QChart* chart = new QChart();
	chart->addSeries(series);
	chart->createDefaultAxes();
	chart->setAxisX(axisX);
	chart->setAxisY(axisY);
	chart->setTitle("histogram");
	chart->setAnimationOptions(QChart::SeriesAnimations);
	chart->legend()->setVisible(false);

	//表的容器
	panelwin->ui.histview->setChart(chart);
	panelwin->ui.histview->setRenderHint(QPainter::Antialiasing);
	panelwin->ui.histview->setVisible(true);

}

void ImageAugmentation::histReg()
{
	QString fileName2 = QFileDialog::getOpenFileName(this,
		tr("Open Regular Image"),
		QDir::currentPath(),
		tr("image") + "(*.jpg *.png *bmp)");
	if (fileName2 != "")
	{
		QImage* image2 = new QImage;
		if (image2->load(fileName2))
		{
			int hist2[256] = { 0 };
			for (int i = 0;i < image2->width(); i++) {
				for (int j = 0;j < image2->height();j++) {

					QColor color = QColor(image2->pixel(i, j));
					hist2[color.red()]++;

				}
			}
			//for (int i = 0;i < 256;i++) {
			//	qDebug()<<"grayPixnum"<<grayPixelNum[i]<<"hist2:" << hist2[i];
			//}
			Utils::HistRegular(grayPixelNum, hist2, panelwin->T);

			TransImage();
			panelwin->drawline();
		}
		delete image2;
	}
}

void ImageAugmentation::histUniform()
{
	Utils::HistUniform(grayPixelNum, panelwin->T);

	TransImage();
	panelwin->drawline();
}

void ImageAugmentation::meanfilter()
{
	QSize size(image->width(), image->height());
	QImage* image2 = new QImage(size, QImage::Format_Indexed8);
	image2->setColorTable(image->colorTable());
	for (int i = 1;i < image->width() - 1;i++) {
		for (int j = 1;j < image->height() - 1;j++) {
			int a[9] = { 0 }, k = 0;

			for (int dx = -1;dx <= 1;dx++) {
				for (int dy = -1;dy <= 1;dy++) {
					a[k++] = image->pixelColor(i + dx, j + dy).red();
				}
			}
			image2->setPixel(i, j, Utils::mean(a, 9));
		}
	}
	for (int i = 1;i < image->width() - 1;i++) {
		for (int j = 1;j < image->height() - 1;j++) {
			qDebug() << "med2" << image2->pixelColor(i, j).red();
			image->setPixel(i, j, image2->pixelColor(i, j).red());
		}
	}
	delete image2;
	scene->addPixmap(QPixmap::fromImage(*image));
	ui.graphicsView->setScene(scene);
	drawHist();
}

void ImageAugmentation::fcolor()
{
	QSize size(image->width(), image->height());
	QImage* image2 = new QImage(size, QImage::Format_RGB32);
	for (int i = 0;i < image->width();i++) {
		for (int j = 0;j < image->height();j++) {
			QRgb rgb = Utils::gray2rgb(image->pixelColor(i, j).red());
			image2->setPixel(i, j, rgb);
		}
	}
	//delete image2;
	scene->addPixmap(QPixmap::fromImage(*image2));
	ui.graphicsView->setScene(scene);
	//drawHist();
}

void ImageAugmentation::minfilter()
{
	QSize size(image->width(), image->height());
	QImage* image2 = new QImage(size, QImage::Format_Indexed8);
	image2->setColorTable(image->colorTable());
	for (int i = 1;i < image->width() - 1;i++) {
		for (int j = 1;j < image->height() - 1;j++) {
			int a[9] = { 0 }, k = 0;

			for (int dx = -1;dx <= 1;dx++) {
				for (int dy = -1;dy <= 1;dy++) {
					a[k++] = image->pixelColor(i + dx, j + dy).red();
				}
			}
			image2->setPixel(i, j, Utils::min(a, 9));
		}
	}
	for (int i = 1;i < image->width() - 1;i++) {
		for (int j = 1;j < image->height() - 1;j++) {
			qDebug() << "med2" << image2->pixelColor(i, j).red();
			image->setPixel(i, j, image2->pixelColor(i, j).red());
		}
	}
	delete image2;
	scene->addPixmap(QPixmap::fromImage(*image));
	ui.graphicsView->setScene(scene);
	drawHist();
}

void ImageAugmentation::gaussfilter()
{
	QSize size(image->width(), image->height());
	QImage* image2 = new QImage(size, QImage::Format_Indexed8);
	image2->setColorTable(image->colorTable());
	for (int i = 1;i < image->width() - 1;i++) {
		for (int j = 1;j < image->height() - 1;j++) {
			int a[9] = { 0 }, k = 0;

			for (int dx = -1;dx <= 1;dx++) {
				for (int dy = -1;dy <= 1;dy++) {
					a[k++] = image->pixelColor(i + dx, j + dy).red();
				}
			}
			image2->setPixel(i, j, Utils::gauss(a, 9));
		}
	}
	for (int i = 1;i < image->width() - 1;i++) {
		for (int j = 1;j < image->height() - 1;j++) {
			qDebug() << "med2" << image2->pixelColor(i, j).red();
			image->setPixel(i, j, image2->pixelColor(i, j).red());
		}
	}
	delete image2;
	scene->addPixmap(QPixmap::fromImage(*image));
	ui.graphicsView->setScene(scene);
	drawHist();
}

void ImageAugmentation::sobelfilter()
{
	QSize size(image->width(), image->height());
	QImage* image2 = new QImage(size, QImage::Format_Indexed8);
	image2->setColorTable(image->colorTable());
	for (int i = 1;i < image->width() - 1;i++) {
		for (int j = 1;j < image->height() - 1;j++) {
			int a[9] = { 0 }, k = 0;

			for (int dx = -1;dx <= 1;dx++) {
				for (int dy = -1;dy <= 1;dy++) {
					a[k++] = image->pixelColor(i + dx, j + dy).red();
				}
			}
			image2->setPixel(i, j, Utils::sobel(a, 9));
		}
	}
	for (int i = 1;i < image->width() - 1;i++) {
		for (int j = 1;j < image->height() - 1;j++) {
			qDebug() << "med2" << image2->pixelColor(i, j).red();
			image->setPixel(i, j, image2->pixelColor(i, j).red());
		}
	}
	delete image2;
	scene->addPixmap(QPixmap::fromImage(*image));
	ui.graphicsView->setScene(scene);
	drawHist();
}

void ImageAugmentation::laplasfilter()
{
	QSize size(image->width(), image->height());
	QImage* image2 = new QImage(size, QImage::Format_Indexed8);
	image2->setColorTable(image->colorTable());
	for (int i = 1;i < image->width() - 1;i++) {
		for (int j = 1;j < image->height() - 1;j++) {
			int a[9] = { 0 }, k = 0;

			for (int dx = -1;dx <= 1;dx++) {
				for (int dy = -1;dy <= 1;dy++) {
					a[k++] = image->pixelColor(i + dx, j + dy).red();
				}
			}
			image2->setPixel(i, j, Utils::laplus(a, 9));
		}
	}
	for (int i = 1;i < image->width() - 1;i++) {
		for (int j = 1;j < image->height() - 1;j++) {
			qDebug() << "med2" << image2->pixelColor(i, j).red();
			image->setPixel(i, j, image2->pixelColor(i, j).red());
		}
	}
	delete image2;
	scene->addPixmap(QPixmap::fromImage(*image));
	ui.graphicsView->setScene(scene);
	drawHist();
}

void ImageAugmentation::medianfilter()
{
	QSize size(image->width(), image->height());
	QImage* image2 = new QImage(size, QImage::Format_Indexed8);
	image2->setColorTable(image->colorTable());
	for (int i = 1;i < image->width() - 1;i++) {
		for (int j = 1;j < image->height() - 1;j++) {
			int a[9] = { 0 }, k=0;

			for (int dx = -1;dx <= 1;dx++) {
				for (int dy = -1;dy <= 1;dy++) {
					a[k++] = image->pixelColor(i+dx, j+dy).red();
				}
			}
			image2->setPixel(i, j, Utils::median(a, 9));
		}
	}
	for (int i = 1;i < image->width() - 1;i++) {
		for (int j = 1;j < image->height() - 1;j++) {
			qDebug() << "med2" << image2->pixelColor(i, j).red();
			image->setPixel(i, j, image2->pixelColor(i, j).red());
		}
	}
	delete image2;
	scene->addPixmap(QPixmap::fromImage(*image));
	ui.graphicsView->setScene(scene);
	drawHist();
}

void ImageAugmentation::maxfilter()
{
	QSize size(image->width(), image->height());
	QImage* image2 = new QImage(size, QImage::Format_Indexed8);
	image2->setColorTable(image->colorTable());
	for (int i = 1;i < image->width() - 1;i++) {
		for (int j = 1;j < image->height() - 1;j++) {
			int a[9] = { 0 }, k = 0;

			for (int dx = -1;dx <= 1;dx++) {
				for (int dy = -1;dy <= 1;dy++) {
					a[k++] = image->pixelColor(i + dx, j + dy).red();
				}
			}
			image2->setPixel(i, j, Utils::max(a, 9));
		}
	}
	for (int i = 1;i < image->width() - 1;i++) {
		for (int j = 1;j < image->height() - 1;j++) {
			qDebug() << "med2" << image2->pixelColor(i, j).red();
			image->setPixel(i, j, image2->pixelColor(i, j).red());
		}
	}
	delete image2;
	scene->addPixmap(QPixmap::fromImage(*image));
	ui.graphicsView->setScene(scene);
	drawHist();
}



void ImageAugmentation::TransImage()
{
	for (int i = 0;i < image->width(); i++) {
		for (int j = 0;j < image->height();j++) {
			QColor color = QColor(image->pixel(i, j));
			int gray = panelwin->T[color.red()];
			image->setPixel(i, j, gray);
		}
	}
	scene->addPixmap(QPixmap::fromImage(*image));
	ui.graphicsView->setScene(scene);
	drawHist();
}

void ImageAugmentation::reloadImage()
{
	image->load(fileName);
	scene = new QGraphicsScene;
	scene->addPixmap(QPixmap::fromImage(*image));
	ui.graphicsView->setScene(scene);
	drawHist();
}
