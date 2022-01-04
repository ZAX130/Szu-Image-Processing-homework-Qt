#include "FFT.h"

FFT::FFT(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    scene1 = new QGraphicsScene;
    scene2 = new QGraphicsScene;
    scene3 = new QGraphicsScene;
    scene4 = new QGraphicsScene;
    //scene->addPixmap(QPixmap::fromImage(*image));
    ui.graphicsView->setScene(scene1);
    ui.graphicsView_2->setScene(scene2);
    ui.graphicsView_3->setScene(scene3);
    ui.graphicsView_4->setScene(scene4);
    connect(ui.pushButton, &QPushButton::clicked, this, &FFT::btn_chooseImg);
}

void FFT::btn_chooseImg() {
	//´ò¿ªÍ¼Æ¬
	fileName = QFileDialog::getOpenFileName(this,
		tr("Open Input Image"),
		QDir::currentPath(),
		tr("image") + "(*.jpg *.png *bmp)");
	if (fileName != "")
	{
        image1 = imread(fileName.toStdString(), IMREAD_GRAYSCALE);
        qDebug() << image1.rows<<image1.cols;
		if (!image1.empty())
		{
            showOriginImage();
            showFFTMagnitudeImage();
            showFFTPhaseImage();
            showIFFT();
		}
	}
}

QImage FFT::cvMat2QImage(const cv::Mat& mat)
{
    QImage img;
    img = QImage((const unsigned char*)(mat.data),
        mat.cols, mat.rows,
        mat.cols * mat.channels(),
        QImage::Format_Grayscale8);

    return img;
}
Mat FFT::QImage2cvMat(QImage image)
{
    Mat mat;
    qDebug() << image.format();
    switch (image.format())
    {
    case QImage::Format_ARGB32:
    case QImage::Format_RGB32:
    case QImage::Format_ARGB32_Premultiplied:
        mat = Mat(image.height(), image.width(), CV_8UC4, (void*)image.constBits(), image.bytesPerLine());
        break;
    case QImage::Format_RGB888:
        mat = Mat(image.height(), image.width(), CV_8UC3, (void*)image.constBits(), image.bytesPerLine());
        cvtColor(mat, mat, COLOR_BGR2RGB);
        break;
    case QImage::Format_Indexed8:
        mat = Mat(image.height(), image.width(), CV_8UC1, (void*)image.constBits(), image.bytesPerLine());
        break;
    }
    return mat;
}

void FFT::showOriginImage()
{
    scene1->clear();
    qimage1 = cvMat2QImage(image1);
    qDebug() << qimage1.width() << qimage1.height();
    scene1->addPixmap(QPixmap::fromImage(qimage1));

}

void FFT::showFFTMagnitudeImage()
{
    scene2->clear();
    fourierTransformImage = Utils::fourierTransform(image1);
    image2 = Utils::getMagnitudeImage(fourierTransformImage);
    qimage2 = cvMat2QImage(image2);
    scene2->addPixmap(QPixmap::fromImage(qimage2));
}
;
void FFT::showFFTPhaseImage()
{
    scene4->clear();
    image4 = Utils::getPhaseImage(fourierTransformImage);
    qimage4 = cvMat2QImage(image4);
    scene4->addPixmap(QPixmap::fromImage(qimage4));
}

void FFT::showIFFT()
{
    scene3->clear();
    image3 = Utils::inverseFourierTransform(fourierTransformImage);
    qimage3 = cvMat2QImage(image3);
    scene3->addPixmap(QPixmap::fromImage(qimage3));
}
