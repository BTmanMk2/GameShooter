#include "SandLevel.h"

SandLevel::SandLevel(const char * filename, int x, int y, int heightMax,
	GameProtocol layer, QGraphicsItem * parent)
	:svg(filename, tris, fills, 0.648), QGraphicsItem(parent),
	bounding(svg.minX, svg.minY, svg.maxX - svg.minX, svg.maxY - svg.minY + 256)
{
	// set the bounding box
	bounding.setHeight(heightMax - y);
	qDebug("Item size: %f", svg.maxX);
	setPos(x, y);
	setZValue(layer);
	setFlag(QGraphicsItem::ItemIsFocusable);
	setFocus();
	riseUp(400);
	startTimer(20);
}

void SandLevel::riseUp(qreal dis)
{
	riseDis += dis;
	QTransform trans;
	trans.translate(0, dis);
	setTransform(trans, true);
	bounding.setHeight(bounding.height() - dis);
}

QRectF SandLevel::boundingRect() const
{
	return QRectF();
}

void SandLevel::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
	int amount = tris.size();
	for (unsigned int i = 0; i < amount; i++)
	{
		painter->setRenderHint(QPainter::Antialiasing, true);
		painter->setPen(QPen(QColor(255, 255, 255, 0), 0));
		painter->setOpacity(tris[i].opacity);
		if (tris[i].fillIndex >= 0)
		{
			painter->setBrush(fills[tris[i].fillIndex].brush);
		}
		QPolygonF qp = tris[i].triangle;
		unsigned int v_amount = qp.size();
		for (unsigned int j = 0; j < v_amount; j++)
		{
			if (tris[i].turbArc[j].second = true)
				qp[j].setY(qp[j].y() + tris[i].turbArc[j].first);
			else
				qp[j].setY(qp[j].y() - tris[i].turbArc[j].first);
		}
		painter->drawPolygon(qp);
	}
}

void SandLevel::timerEvent(QTimer * timer)
{
}

void SandLevel::keyReleaseEvent(QKeyEvent * event)
{
}
