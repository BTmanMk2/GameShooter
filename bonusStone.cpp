#include "bonusStone.h"



bonusStone::bonusStone(QImage* stoneImg, QPointF birthPos, QPointF shootPos, int stoneID, int playerID, StoneManager* parentManager)
{
	this->parentManager = parentManager;
	setPixmap(QPixmap::fromImage(*stoneImg));
	setOffset(-stoneImg->width() / 2.0, -stoneImg->height() / 2.0);
	this->playerID = playerID;
	this->stoneID = stoneID;
	this->birthPos = birthPos;
	this->dropPos = shootPos;

}


bonusStone::~bonusStone()
{
}
