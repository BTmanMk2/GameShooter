#pragma once
#include<QGraphicsItem>
#include<QPainter>
#include<QTimer>
#include<QBrush>
#include<time.h>
#include<QMutex>
#include<QTextStream>
#include"Protocol.h"
#include"MySVG.h"

class ColorRepo
{
public:
	QVector<QString> colors;
	void readColors();
	int getSize();
	QString getRandomColor();
	QString getAccRandomColor(int num);
	int accbound = 5;
};

class SandLevel : public QGraphicsItem, public QObject
{
public:
	/************************ User Define ***************************/
	SandLevel(const char* filename, int x, int y, int heightMax = 768,
		GameProtocol layer = LAYER_SAND, QGraphicsItem* parent = Q_NULLPTR);

	void riseUp(qreal dis, bool record = false);

	void initSand();

	void generateSandLine();

	void computeSandLine(int up, int down);

	void computeSandTurb();

	void computeColorLine();
	/************************ Override *****************************/
	QRectF boundingRect() const;
	void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);
	void timerEvent(QTimerEvent* timer);
	void keyReleaseEvent(QKeyEvent* event);

	void Update();
private:
	//QVector<SVGData::SVGTriangle> tris;
	//QVector<SVGData::SVGFill> fills;
	//SVGData svg;
	QRectF bounding;
	qreal riseDis = 600;
	qreal supportDis;

	qreal right = 0;
	qreal left = 0;

	// generated lines part
	ColorRepo colorRepo;
	struct MyPoint
	{
		QPointF point;
		QPointF turb;
		int flags;		// above 0 need coop, below 0 is angle point
		QPointF getReal() { return point + turb; }
		MyPoint(QPointF& p1, QPointF& p2, int flag = 0) 
			:point(p1), turb(p2), flags(flag) {}
		MyPoint() {};
	};

	QVector<QVector<MyPoint>> lines;
	QVector<QVector<QString>> colors;
	QVector<qreal> peak;
	int minC = 10, maxC = 15;

	int currentY = 30;
	int stepY = 50;
	int stepTime = 10;

	int maxX = 511;						// to the rightest place
	int turbX = 2;						// cor-x's turb
	int turbY = 12;						// cor-y's turb
	int minConTri = 4, maxConTri = 8;	// conversed triangle
	int midHeight = 50;					// peak height

	QMutex lock;
	int stored_dis = 0;

	double myDoubleRand()
	{
		double k = qrand() * 1.0 / RAND_MAX;
		return k;
	}

	int myIntRand(int num)
	{
		return qrand() % num;
	}

	int mySignedRand(int num)
	{
		int result = num?qrand() % num:num;
		if (qrand() % 2) return -result;
		else return result;
	}

	//GameManager* a;
};

