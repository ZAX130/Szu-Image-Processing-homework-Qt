 #include "Ffilter.h"

Ffilter::Ffilter(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

    fileName = "";

    scene1 = new QGraphicsScene;
    scene2 = new QGraphicsScene;
    scene3 = new QGraphicsScene;
    scene4 = new QGraphicsScene;
    scene5 = new QGraphicsScene;

    ui.graphicsView->setScene(scene1);
    ui.graphicsView_2->setScene(scene2);
    ui.graphicsView_3->setScene(scene3);
    ui.graphicsView_4->setScene(scene4);
    ui.graphicsView_5->setScene(scene5);

    connect(ui.btn_chooseImg, &QPushButton::clicked, this, &Ffilter::btn_chooseImg);
    connect(ui.btn_bwHigh, &QPushButton::clicked, this, &Ffilter::butterworth_high);
    connect(ui.btn_bwLow, &QPushButton::clicked, this, &Ffilter::butterworth_Low);
    connect(ui.btn_ideaHigh, &QPushButton::clicked, this, &Ffilter::ideal_high);
    connect(ui.btn_ideaLow, &QPushButton::clicked, this, &Ffilter::ideal_low);
    connect(ui.btn_homo, &QPushButton::clicked, this, &Ffilter::homofilter);
    connect(ui.btn_reset, &QPushButton::clicked, this, &Ffilter::reset);
    connect(ui.slider_D0, &QSlider::valueChanged, this, &Ffilter::D0silderChange);
    connect(ui.slider_n, &QSlider::valueChanged, this, &Ffilter::nsilderChange);
    connect(ui.slider_gammaH, &QSlider::valueChanged, this, &Ffilter::gammaHsilderChange);
    connect(ui.slider_gammaL, &QSlider::valueChanged, this, &Ffilter::gammaLsilderChange);
}

void Ffilter::btn_chooseImg() {
    //打开图片
    fileName = QFileDialog::getOpenFileName(this,
        tr("Open Input Image"),
        QDir::currentPath(),
        tr("image") + "(*.jpg *.png *bmp)");
    if (fileName != "")
    {
        image1 = imread(fileName.toStdString(), IMREAD_GRAYSCALE);
        qDebug() << image1.rows << image1.cols;
        if (!image1.empty())
        {
            showOriginImage();
            showFFTMagnitudeImage();
            showFFTPhaseImage();
            showIFFT();
        }
    }
}

QImage Ffilter::cvMat2QImage(const cv::Mat& mat)
{
    QImage img;
    img = QImage((const unsigned char*)(mat.data),
        mat.cols, mat.rows,
        mat.cols * mat.channels(),
        QImage::Format_Grayscale8);

    return img;
}
Mat Ffilter::QImage2cvMat(QImage image)
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

void Ffilter::showOriginImage()
{
    scene1->clear();
    qimage1 = cvMat2QImage(image1);
    qDebug() << qimage1.width() << qimage1.height();
    scene1->addPixmap(QPixmap::fromImage(qimage1));
    fourierTransformImage = Utils::fourierTransform(image1);
    origin_fourierTransformImage = fourierTransformImage.clone();
}

void Ffilter::showFFTMagnitudeImage()
{
    scene3->clear();
    
    image3 = Utils::getMagnitudeImage(fourierTransformImage);
    qimage3 = cvMat2QImage(image3);
    scene3->addPixmap(QPixmap::fromImage(qimage3));
}
;
void Ffilter::showFFTPhaseImage()
{
    scene4->clear();
    image4 = Utils::getPhaseImage(fourierTransformImage);
    qimage4 = cvMat2QImage(image4);
    scene4->addPixmap(QPixmap::fromImage(qimage4));
}

void Ffilter::showIFFT()
{
    scene2->clear();
    image2 = Utils::inverseFourierTransform(fourierTransformImage);
    qimage2 = cvMat2QImage(image2);
    scene2->addPixmap(QPixmap::fromImage(qimage2));
}

void Ffilter::butterworth_Low()
{
    if (fileName == "") return;
    //d0过大图像,滤波器频谱会全黑,本来应该是全白,可能是归一化引起的
    int n = ui.slider_n->value();
    int d0 = ui.slider_D0->value();
    scene5->clear();
    Mat bw_kernel = Utils::getButterworth_low_kernel(image1, d0, n);
    image5 = Utils::ToShowFormat(bw_kernel);
    qimage5 = cvMat2QImage(image5);
    scene5->addPixmap(QPixmap::fromImage(qimage5));
    fourierTransformImage = Utils::filtered_fourier(bw_kernel, origin_fourierTransformImage);
    showFFTMagnitudeImage();
    showFFTPhaseImage();
    showIFFT();
}

void Ffilter::butterworth_high()
{
    if (fileName == "") return;
    int n = ui.slider_n->value();
    int d0 = ui.slider_D0->value();
    scene5->clear();
    Mat bw_kernel = Utils::getButterworth_high_kernel(image1, d0, n);
    image5 = Utils::ToShowFormat(bw_kernel);
    qimage5 = cvMat2QImage(image5);
    scene5->addPixmap(QPixmap::fromImage(qimage5));

    fourierTransformImage = Utils::filtered_fourier(bw_kernel, origin_fourierTransformImage);
    showFFTMagnitudeImage();
    showFFTPhaseImage();
    showIFFT();
}

void Ffilter::ideal_low()
{
    if (fileName == "") return;
    //d0过大图像,滤波器频谱会全黑,本来应该是全白,可能是归一化引起的
    int d0 = ui.slider_D0->value();
    scene5->clear();
    Mat bw_kernel = Utils::getideal_low_kernel(image1, d0);
    image5 = Utils::ToShowFormat(bw_kernel);
    qimage5 = cvMat2QImage(image5);
    scene5->addPixmap(QPixmap::fromImage(qimage5));
    fourierTransformImage = Utils::filtered_fourier(bw_kernel, origin_fourierTransformImage);
    showFFTMagnitudeImage();
    showFFTPhaseImage();
    showIFFT();
}

void Ffilter::ideal_high()
{
    if (fileName == "") return;
    int d0 = ui.slider_D0->value();
    scene5->clear();
    Mat bw_kernel = Utils::getideal_high_kernel(image1, d0);
    image5 = Utils::ToShowFormat(bw_kernel);
    qimage5 = cvMat2QImage(image5);
    scene5->addPixmap(QPixmap::fromImage(qimage5));

    fourierTransformImage = Utils::filtered_fourier(bw_kernel, origin_fourierTransformImage);
    showFFTMagnitudeImage();
    showFFTPhaseImage();
    showIFFT();
}

void Ffilter::homofilter()
{
    if (fileName == "") return;
    int n = ui.slider_n->value();
    int d0 = ui.slider_D0->value();
    double gammaH = ui.slider_gammaH->value()/10.0;
    double gammaL = ui.slider_gammaL->value()/10.0;
    scene5->clear();
    Mat homo_kernel = Utils::gethomo_kernel(image1, gammaH, gammaL, d0, n);
    image5 = Utils::ToShowFormat(homo_kernel);
    qimage5 = cvMat2QImage(image5);
    scene5->addPixmap(QPixmap::fromImage(qimage5));
    fourierTransformImage = Utils::LogfourierTransform(image1);
    fourierTransformImage = Utils::filtered_fourier(homo_kernel, fourierTransformImage);
    showFFTMagnitudeImage();
    showFFTPhaseImage();
    scene2->clear();
    image2 = Utils::expinverseFourierTransform(fourierTransformImage);
    qimage2 = cvMat2QImage(image2);
    scene2->addPixmap(QPixmap::fromImage(qimage2));
}

void  Ffilter::D0silderChange() {
    int d0 = ui.slider_D0->value();
    ui.lab_D0->setText(QString::number(d0));
}
void  Ffilter::nsilderChange() { 
    int n = ui.slider_n->value();                                  
    ui.lab_n->setText(QString::number(n));
}
void  Ffilter::gammaHsilderChange() {
    int gammaH = ui.slider_gammaH->value();
    ui.lab_gammaH->setText(QString::number(gammaH/10.0));
}
void  Ffilter::gammaLsilderChange() {
    int gammaL = ui.slider_gammaL->value();
    ui.lab_gammaL->setText(QString::number(gammaL / 10.0));
}
void Ffilter::reset()
{
    image1 = imread(fileName.toStdString(), IMREAD_GRAYSCALE);
    if (!image1.empty())
    {
        showOriginImage();
        showFFTMagnitudeImage();
        showFFTPhaseImage();
        showIFFT();
        scene5->clear();
    }
}
