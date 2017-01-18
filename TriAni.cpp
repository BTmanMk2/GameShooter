#include "TriAni.h"

void MultiTriItem::setTris(QVector<SVGData::SVGTriangle>& tts)
{
	tris = tts;
}

QRectF MultiTriItem::boundingRect() const
{
	return boundingbox;
}

void MultiTriItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
	painter->setOpacity(0.5);
	painter->setBrush(QColor(0, 0, 0));
	painter->drawRect(10, 10, 100, 100);
	//for (int i = 0; i < tris.size(); i++)
	//{
	//	painter->drawPolygon(tris[i].triangle);
	//}
}
