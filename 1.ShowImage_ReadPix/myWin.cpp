#include "myWin.h"

myWin::myWin(QWidget* parent) 
	: QWidget(parent) {
	ui.setupUi(this);
	ui.graphicsView->setVisible(false);
	ui.label->setVisible(false);
	ui.label_3->setVisible(false);
	ui.label_4->setVisible(false);
	ui.label_5->setVisible(false);

	//槽函数
	connect(ui.graphicsView, &QMyGraphicsView::updateItemPos, this, &myWin::updateItemPos);
	connect(ui.pushButton, &QPushButton::clicked, this, &myWin::btn_chooseImg);
}
myWin::~myWin() {
	
}

void myWin::btn_chooseImg() {
	//打开图片
	QString fileName = QFileDialog::getOpenFileName(this,
		tr("Open Input Image"),
		QDir::currentPath(),
		tr("image") + "(*.jpg *.png *bmp)");
	if (fileName != "")
	{
		image = new QImage;
		ui.graphicsView->setVisible(true);
		ui.label->setVisible(true);
		ui.label_3->setVisible(true);
		ui.label_4->setVisible(true);
		ui.label_5->setVisible(true);
		if (image->load(fileName))
		{
			QGraphicsScene* scene = new QGraphicsScene;
			scene->addPixmap(QPixmap::fromImage(*image));
			ui.graphicsView->setScene(scene);
			//重新
			ui.graphicsView->setFixedSize(image->width() + 2, image->height() + 2);
			setFixedSize(image->width() + 25, image->height() + 130);
			ui.graphicsView->move(50, 50);
		}
	}
}
void myWin::updateItemPos(QPointF pos)
{
	//根据获得的坐标,给出RGB像素值
	int x = pos.x();
	int y = pos.y();
	QString str = QString("coordinate:       ") + "(" + QString::number(x) + "," + QString::number(y) + ")";
	ui.label->setText(str);
	QColor oldColor = QColor(image->pixel(x, y));
	ui.vR->setText("  "+QString::number(oldColor.red()));
	ui.vG->setText("  " + QString::number(oldColor.green()));
	ui.vB->setText("  " + QString::number(oldColor.blue()));

}