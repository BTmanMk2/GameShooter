#pragma once
#include<qgraphicsitem.h>
#include<qpixmap.h>
#include<qpainter.h>
#include<qgraphicsscene.h>
#include"Protocol.h"

class BgManager : public QObject, public QGraphicsItem
{
public:
	/*********************** User Define ************************/
	BgManager(const char* bgfile, const char* holefile, QGraphicsScene* scene, int x = 0, int y = 0,
		GameProtocol bglayer = LAYER_BG, GameProtocol holelayer = LAYER_HOLE,
		QGraphicsItem* parent = Q_NULLPTR);

	void holePos(int x, int y);
	/*********************** Override ***************************/
	QRectF boundingRect() const;
	void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = Q_NULLPTR);
private:
	QGraphicsPixmapItem bg;
	QGraphicsPixmapItem hole;
	int rx, ry;
};