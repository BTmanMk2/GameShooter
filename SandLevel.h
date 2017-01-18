#pragma once
#include<qgraphicsitem.h>
#include<qpainter>
#include<qtimer.h>
#include<qbrush.h>
#include<time.h>
#include"Protocol.h"
#include"MySVG.h"

class SandLevel : public QGraphicsItem, public QObject
{
public:
	/************************ User Define ***************************/
	SandLevel(const char* filename, int x, int y, int heightMax = 768,
		GameProtocol layer = LAYER_WATER, QGraphicsItem* parent = Q_NULLPTR);

	void riseUp(qreal dis);

	/************************ Override *****************************/
	QRectF boundingRect() const;
	void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);
	void timerEvent(QTimer* timer);
	void keyReleaseEvent(QKeyEvent* event);
private:
	QVector<SVGData::SVGTriangle> tris;
	QVector<SVGData::SVGFill> fills;
	SVGData svg;
	QRectF bounding;

	int riseDis = 0;

	double myRand()
	{
		double k = qrand() * 1.0 / RAND_MAX;
		return k;
	}
};