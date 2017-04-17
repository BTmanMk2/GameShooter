#include "stone.h"
#include <QGraphicsScene>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <QTransform>
#include <cmath>
#include <QtGlobal>
#include "stonemanager.h"
//#include "fragment.h"


const qreal birthInterval = 100;
const qreal popInterval = 40;
const qreal popAcclRatio = 0.4;

int randint(int a, int b) {
	return qrand() % (b - a + 1) + a;
}

void Stone::removeThis() {
	parentManager->removeStone(this->stoneID);
	scene()->removeItem(this);
	delete this;
}
/* QVector<SVGData::SVGTriangle>*tris, QVector<SVGData::SVGFill>*fills,*/
Stone::Stone(QImage* stoneImg,	QPointF birthPos, QPointF shootPos, int stoneID, int playerID, StoneManager* parentManager)
	:QObject(), QGraphicsPixmapItem() {
	this->parentManager = parentManager;
	setPixmap(QPixmap::fromImage(*stoneImg));
	setOffset(-stoneImg->width() / 2.0, -stoneImg->height() / 2.0);
	this->playerID = playerID;
	this->stoneID = stoneID;
	this->birthPos = birthPos;
	this->dropPos = shootPos;

	QPointF originPos;
	originPos.setX(randint(birthPos.x() - 20, birthPos.x() + 20));
	originPos.setY(birthPos.y() - 5);
	this->setPos(originPos);
	this->setScale(birthScale);
	this->setShapeMode(QGraphicsPixmapItem::MaskShape);

	

	speed = parentManager->getCurrentSpeed();	//slowest speed **need to replace

	stage = STONE_BIRTH;

	birthMov.setX((birthPos.x() - originPos.x()) / birthInterval);
	birthMov.setY((birthPos.y() - originPos.y()) / birthInterval);

	scaleStep = (dropScale - birthScale) / birthInterval;

	popMov.setX((dropPos.x() - birthPos.x()) / popInterval);
	popMov.setY((dropPos.y() - birthPos.y()) / popInterval);
	popMovAccl.setX(popMov.x() / popAcclRatio);
	popMovAccl.setY(popMov.y() / popAcclRatio);

	/*this->tris = tris;
	this->fills = fills;*/

	timer = new QTimer(this);
	QObject::connect(timer, SIGNAL(timeout()), this, SLOT(drop()));
	timer->start(20);
}



void Stone::speedUp()
{
	speed += speedStep;
}

void Stone::speedDown()
{
	if (speed > 1) {
		speed -= speedStep;
	}
}

void Stone::start()
{
	//timer->start(15);
}

void Stone::stop() {
	timer->stop();
}

void Stone::mousePressEvent(QGraphicsSceneMouseEvent  *event)
{
	if (enableShoot == true) {
		std::cout << "clicked" << std::endl;
		//Game->addScore(score, groupID);
		parentManager->addScore(10);
		
		parentManager->playfrag(stoneType, QPointF(x(), y()));
		removeThis();
		//Game->stoneManager1->playFragment(QPointF loc, unsigned int stoneType);
	}
}

void Stone::drop() {
	//std::cout << x() << "  " << y() << std::endl;
	/*QPointF gp = parentManager->getPoint();
	QMouseEvent mouseEvent(QMouseEvent::MouseButtonPress, gp, Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
	QCoreApplication::sendEvent(this, &mouseEvent);
	QCoreApplication::sendEvent(scene(), &mouseEvent);*/
	if (stage == STONE_BIRTH) {
		setPos(x() + birthMov.x(), y() + birthMov.y());
		setScale(scale() + scaleStep);
		if (std::abs(x() - birthPos.x()) < 1 && std::abs(y() - birthPos.y()) < 0.8) {
			setScale(dropScale);
			//this->setZValue(LAYER_STONE_FRONT);/*to be deleted*/
			stage = STONE_ABOVE_LINE;
			parentManager->popFront(stoneID);

		}
	}
	else if (stage == STONE_WAIT) {
		if (parentManager->getCurrentStack() < parentManager->getMaxStack()) {
			stage = STONE_ABOVE_LINE;
			parentManager->popFront(stoneID);
		}
	}
	else if (stage == STONE_ABOVE_LINE) {
		if (y() > dropPos.y()) {
			stage = STONE_UNDER_LINE;
		}
		setPos(x() + popMov.x(), y() + popMov.y());
		popMov.setX(popMov.x() + popMovAccl.x());
		popMov.setY(popMov.y() + popMovAccl.y());
	}
	else if (stage == STONE_UNDER_LINE) {
		enableShoot = true;
		setPos(x(), y() + speed);
		if (y() > parentManager->getWaterHeight()) {
			parentManager->missOne();
			removeThis();
			stage = STONE_FRAG;
			enableShoot = false;
		}
	}
	else {

	}
}