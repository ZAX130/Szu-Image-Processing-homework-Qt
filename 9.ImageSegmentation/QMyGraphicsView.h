#ifndef QMYGRAPHICSVIEW_H
#define QMYGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QGraphicsItem>
#include <QMouseEvent>

class QMyGraphicsView : public QGraphicsView
{
	Q_OBJECT

signals:
	void updateItemPos(QPointF& pos);
	//发送坐标的信号函数
public:
	QMyGraphicsView(QWidget* parent = nullptr);
protected:
	void mousePressEvent(QMouseEvent* event);
	//重写监听函数,使用此函数发送坐标信号
};

#endif // QMYGRAPHICSVIEW_H