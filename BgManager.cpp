#include "BgManager.h"

BgManager::BgManager(const char * bgfile, const char * holefile, 
	int x, int y, GameProtocol bglayer, GameProtocol holelayer,
	QGraphicsItem * parent)
	:bg(QPixmap(bgfile), this), hole(QPixmap(holefile), this), QGraphicsItem(parent)
{
	setPos(x, y);
	setZValue(bglayer);
	hole.setPos(174, 26);
	hole.setZValue(holelayer);
}

QRectF BgManager::boundingRect() const
{
	return QRectF(0, 0, 512, 768);
}

void BgManager::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
}
