#pragma once
#include "stone.h"
class bonusStone :
	public Stone
{
	Q_OBJECT
	/*unsigned int playerID;
	unsigned int stoneID;
	unsigned int stoneType;*/

	/*QPointF birthPos;
	QPointF dropPos;*/


	/*QTimer* timer;
	QMediaPlayer* stonePlayer;

	StoneManager* parentManager;*/


public:
	bonusStone(QImage* stoneImg, QPointF birthPos, QPointF shootPos, int stoneID, int playerID, StoneManager* parentManager);
	~bonusStone();
};

