#pragma once

#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QObject>
#include <vector>
#include <string>
#include "stone.h"
#include "Protocol.h"
#include <QImage>
#include <QRectF>
#include <QtGlobal>
#include <QVector>

#include "fragment.h"

//#include <QMediaPlayer>
//#include "MySVG.h"


class GameManager;

class StoneManager :public QObject, public QGraphicsRectItem
{
	Q_OBJECT

		GameProtocol playerID;

	QPointF originPos;
	QPointF birthPos;
	int linePos;
	int shootLineY;

	int level;

	std::vector<QImage*>stoneImages;

	std::vector<Stone*>stonePointers;
	std::vector<int>stoneLayer;

	QTimer* timer;
	QTimer* timer2;

	GameManager* parentManager;

	int scoreGap = 0;
	double currentSpeed = 1.2;

	unsigned int maxStack = 15;
	unsigned int curStack = 0;
	unsigned int inRangeStack = 0;


	fragment *frag;		//fragment effect

	unsigned int combo;		//combo count

protected:
	unsigned int genStoneID();
	int genLayer();

public:
	StoneManager(QPointF position, GameProtocol playerID, GameManager* parentManager);
	~StoneManager();
	void keyPressEvent(QKeyEvent* event);
	void removeStone(unsigned int stoneID);
	void popFront(unsigned int stoneID);
	void levelUp();
	void stop();
	void start();
	void reset();
	void addScore(int score);
	void missOne();
	int getCombo();
	int getWaterHeight();
	int getCurrentScore();
	double getCurrentSpeed();
	int getCurrentStack() {
		return curStack;
	}
	int getMaxStack() {
		return maxStack;
	}
	void addFrag() {
		scene()->addItem(frag);
	}
	void playfrag(int stoneIndex, QPointF pos) {
		frag->addFrag(stoneIndex, pos-originPos);
	}
	//QPointF getPoint();
	public slots:
	void spawn();
};
