#include "RGB2HSV.h"

RGB2HSV::RGB2HSV(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
	panelwin = new PanelWin();
	connect(ui.pushButton, &QPushButton::clicked, this, &RGB2HSV::btn_chooseImg);
	connect(panelwin, &PanelWin::sliderChange, this, &RGB2HSV::updateImage);//
	connect(panelwin, &PanelWin::resetScene, this, &RGB2HSV::reloadImage);//
}

void RGB2HSV::btn_chooseImg() {
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

			scene = new QGraphicsScene;
			scene->addPixmap(QPixmap::fromImage(*image));
			ui.graphicsView->setScene(scene);
			//重新布局控件
			ui.graphicsView->setFixedSize(image->width() + 2, image->height() + 2);
			setFixedSize(image->width() + 10, image->height() + 45);
			ui.graphicsView->move(5, 5);
			ui.pushButton->setFixedSize(image->width() + 2, 30);
			ui.pushButton->move(5,image->height()+10);

			panelwin->show();
			double mR = 0.0, mG = 0.0, mB = 0.0, mH = 0.0, mS = 0.0, mV = 0.0;
			for (int i = 0;i < image->width(); i++) {
				for (int j = 0;j < image->height();j++) {
					RGB rgb;
					HSV hsv;
					QColor color = QColor(image->pixel(i, j));
					rgb.r = color.red();
					rgb.g = color.green();
					rgb.b = color.blue();
					hsv = Utils::RGB2HSV(rgb);
					mR += rgb.r;
					mG += rgb.g;
					mB += rgb.b;
					mH += hsv.h;
					mS += hsv.s;
					mV += hsv.v;
				}
			}
			panelwin->ui.mR->setText(QString::number(mR / (image->width() * image->height())));
			panelwin->ui.mG->setText(QString::number(mG / (image->width() * image->height())));
			panelwin->ui.mB->setText(QString::number(mB / (image->width() * image->height())));
			panelwin->ui.mH->setText(QString::number(mH / (image->width() * image->height())));
			panelwin->ui.mS->setText(QString::number(mS / (image->width() * image->height())));
			panelwin->ui.mV->setText(QString::number(mV / (image->width() * image->height())));
		}
	}	
}

void RGB2HSV::updateImage()
{
	image->load(fileName);
	scene->addPixmap(QPixmap::fromImage(*image));
	ui.graphicsView->setScene(scene);
	double mR=0.0, mG=0.0, mB=0.0,mH=0.0,mS=0.0,mV=0.0;
	for (int i = 0;i < image->width(); i++) {
		for (int j = 0;j < image->height();j++) {
			RGB rgb;
			HSV hsv;
			//qDebug() << i << j << endl;
			QColor color = QColor(image->pixel(i, j));
			//qDebug()<<"rgb" << color.red() << color.green() << color.blue() << endl;
			rgb.r = color.red();
			rgb.g = color.green();
			rgb.b = color.blue();

			hsv=Utils::RGB2HSV(rgb);
			//qDebug() << "hsv" << hsv.h << hsv.s << hsv.v << endl;
			HSV deltaHsv;
			deltaHsv.h = panelwin->ui.hSlider->value();
			deltaHsv.s = panelwin->ui.sSlider->value() / 100.0;
			deltaHsv.v = panelwin->ui.vSlider->value() / 100.0;
			//qDebug() <<"delta" << deltaHsv.h << deltaHsv.s << deltaHsv.v << endl;
			hsv = Utils::changeHSV(hsv, deltaHsv);
			//qDebug() << "changehsv" << hsv.h << hsv.s << hsv.v << endl;
			rgb = Utils::HSV2RGB(hsv);
			QColor new_color = QColor(rgb.r, rgb.g, rgb.b).rgb();
			//qDebug() << "new_rgb" << rgb.r << rgb.g << rgb.b << endl;
			mR += rgb.r;
			mG += rgb.g;
			mB += rgb.b;
			mH += hsv.h;
			mS += hsv.s;
			mV += hsv.v;
			image->setPixelColor(i, j, new_color);
		}
	}
	scene->addPixmap(QPixmap::fromImage(*image));
	ui.graphicsView->setScene(scene);
	panelwin->ui.mR->setText(QString::number(mR / (image->width() * image->height())));
	panelwin->ui.mG->setText(QString::number(mG / (image->width() * image->height())));
	panelwin->ui.mB->setText(QString::number(mB / (image->width() * image->height())));
	panelwin->ui.mH->setText(QString::number(mH / (image->width() * image->height())));
	panelwin->ui.mS->setText(QString::number(mS / (image->width() * image->height())));
	panelwin->ui.mV->setText(QString::number(mV / (image->width() * image->height())));
}

void RGB2HSV::reloadImage()
{
	image->load(fileName);
	scene = new QGraphicsScene;
	scene->addPixmap(QPixmap::fromImage(*image));
	ui.graphicsView->setScene(scene);
	//重新布局控件
	ui.graphicsView->setFixedSize(image->width() + 2, image->height() + 2);
	setFixedSize(image->width() + 10, image->height() + 45);
	ui.graphicsView->move(5, 5);
	ui.pushButton->setFixedSize(image->width() + 2, 30);
	ui.pushButton->move(5, image->height() + 10);

	panelwin->show();
	double mR = 0.0, mG = 0.0, mB = 0.0, mH = 0.0, mS = 0.0, mV = 0.0;
	for (int i = 0;i < image->width(); i++) {
		for (int j = 0;j < image->height();j++) {
			RGB rgb;
			HSV hsv;
			QColor color = QColor(image->pixel(i, j));
			rgb.r = color.red();
			rgb.g = color.green();
			rgb.b = color.blue();
			hsv = Utils::RGB2HSV(rgb);
			mR += rgb.r;
			mG += rgb.g;
			mB += rgb.b;
			mH += hsv.h;
			mS += hsv.s;
			mV += hsv.v;
		}
	}
	panelwin->ui.mR->setText(QString::number(mR / (image->width() * image->height())));
	panelwin->ui.mG->setText(QString::number(mG / (image->width() * image->height())));
	panelwin->ui.mB->setText(QString::number(mB / (image->width() * image->height())));
	panelwin->ui.mH->setText(QString::number(mH / (image->width() * image->height())));
	panelwin->ui.mS->setText(QString::number(mS / (image->width() * image->height())));
	panelwin->ui.mV->setText(QString::number(mV / (image->width() * image->height())));
}
