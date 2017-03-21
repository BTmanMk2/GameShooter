#pragma once
#include<qgraphicsitem.h>
#include<qpainter>
#include<qtimer.h>
#include<qbrush.h>
#include<time.h>
#include"Protocol.h"
#include"MySVG.h"

class WaterLevel : public QGraphicsItem, public QObject
{
public:
	/***************************** User Define **************************/
	/**
	* Constructor
	*	filename	char*	the name of the svg file
	*	x			int		the coorX of the location
	*	y			int		the coorY of the location
	*	layer		GamePro	the layer of the item
	*	parent		QGraIt	the parent graphics item
	* Build up a Water Level Component to show
	*/
	WaterLevel(const char* filename, int x, int y, int heightMax = 768,
		GameProtocol layer = LAYER_WATER, QGraphicsItem* parent = Q_NULLPTR);

	/**
	* Rise Up
	*	dis			qreal	the dis need to transfer. hint: the coord direction is unusual
	* Rise up by a dis, in fact we add the dis to the coord-Y
	*/
	void riseUp(qreal dis);


	/***************************** Override *****************************/
	QRectF boundingRect() const;
	void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = Q_NULLPTR);
	void timerEvent(QTimerEvent* timer);
	void keyReleaseEvent(QKeyEvent* event);
private:
	QVector<SVGData::SVGTriangle> tris;
	QVector<SVGData::SVGFill> fills;
	SVGData svg;
	QRectF bounding;

	void objDrawer(QPainter* painter);
	void shuffle1();
	void shuffle2();

	void addRgb(char r, char g, char b);
	void saveRgb();

	qreal step = 0.3;
	double turb = 50;
	int riseDis = 0;

	double myRand()
	{
		double k = qrand() * 1.0 / RAND_MAX;
		return k;
	}
};
