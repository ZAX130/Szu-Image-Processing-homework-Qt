#include "QMyGraphicsView.h"

QMyGraphicsView::QMyGraphicsView(QWidget* parent) : QGraphicsView(parent)
{

}

void QMyGraphicsView::mouseMoveEvent(QMouseEvent* event) {
    if (QGraphicsItem* item = itemAt(event->pos()))
    {
        QPointF sceneMousePos = this->mapToScene(event->pos());
        QPointF itemMousePos = item->mapFromScene(sceneMousePos);
        emit updateItemPos(itemMousePos);
    }
    else {
        QPointF p(0, 0);
        emit updateItemPos(p);
    }
}



