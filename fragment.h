#pragma once
#include <QGraphicsItem>
#include <QObject>
#include <QPainter>
#include <QTimer>
#include <QBrush>
#include <time.h>
#include <vector>
#include <sstream>
#include <iostream>
#include <qdebug.h>

#include "MySVG.h"
#include "Protocol.h"


class fragment:public QGraphicsItem, public QObject
{
	//Q_OBJECT
	QRectF bounding;
	int stoneType;
	QPointF birthpos;

	int maxStep = 7;
	double scalep = 0.728;

	void objDrawer(QPainter* painter);
	//void shuffle1();

	//fragment storage
	std::vector<QVector<SVGData::SVGTriangle>>stoneTris;
	std::vector<QVector<SVGData::SVGFill>>stoneFills;

	//animation fragments
	struct fragStone {
		QVector<SVGData::SVGTriangle>tris;
		QVector<SVGData::SVGFill>fills;
		QPointF center;
		std::vector<QPointF>directions;
		int stepcnt;
	};

	std::vector<fragStone>stones;

public:
	/***************************** Override *****************************/
	QRectF boundingRect() const;
	void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = Q_NULLPTR);
	void timerEvent(QTimerEvent* timer);
	void keyReleaseEvent(QKeyEvent* event);

	/**********************************************/
	fragment(int type, QPointF pos, QGraphicsItem* parent = Q_NULLPTR);

	void addFrag(int type, QPointF pos);

	~fragment();

	
};

