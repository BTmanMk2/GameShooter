/*
* Game Shooter
*  - Author:	Sturmfy Xu
*  - Date:		2017/1/17
*  - Version:	0.1
*	This Game is Designed within this entry class called GameManager. This class
* try to control the logical part of the game, and direct the whole system. Every
* component is regard as a child item, who provides function & event to support
* what we need.
--------------------------------------------------------------------------------
* Class Structure:
*	GameManager	-- WaterLevel
*				-- BgManager
*				-- MarkItem
*
*/
#pragma once
#include"Protocol.h"
#include"WaterLevel.h"
#include"BgManager.h"
#include<qwidget.h>
#include<qgraphicsscene.h>
#include<qgraphicsview.h>
#include<qfontdatabase.h>
#include<qfont.h>
#include<qlabel.h>
#include<sstream>
#include<qmutex.h>

#ifdef _OTHER_MANAGER
class FallManager;
class SandLevel;
#endif

class MarkItem;
class GameManager : public QWidget
{
public:
	/**
	* Constructor
	*	width	int	:the width of the game
	*	height	int	:the height of the game
	*	pro	GamePro	:the game mode
	*	parent	QWi	:the parent widget of the game
	* Build up the whole system.
	*/
	GameManager(int width, int height, GameProtocol pro = SINGLE_PLAYER, QWidget* parent = nullptr);
	~GameManager();
private:
	/********************* Managers ***********************/
#ifdef _OTHER_MANAGER
	FallManager* fm;
	SandLevel* sandLevel;
#endif
	MarkItem* mi;
	BgManager* bm;
	WaterLevel* waterLevel;

	/********************* Render ************************/
	QGraphicsScene* scene;
	QGraphicsView* view;
};

class MarkItem : public QGraphicsItem, public QObject
{
public:
	/**
	* Constructor
	*	fontfile	char*	the file of the font, if null, it would not load a new font
	*	font_size	int		the size of the component font
	*	x			int		the coord-X of the component
	*	y			int		the coord-Y of the component
	*	layer		GamePro	the layer it should lay on
	*	parent		QGraIt	the parent graphics item
	* read self defined font from given file and build up the font
	*/
	MarkItem(const char* fontfile, int font_size, int x, int y, GameProtocol layer = LAYER_WATER, QGraphicsItem* parent = Q_NULLPTR);

	/**
	* Set Family
	*	familyname	char*	if you didn't specify the fontfile in constructor(NULL), you can set the family name.
	* One use this function when the constructor's fontfile set to NULL, and you can set the font through this way,
	* especially when you have multi-MarkItem, then you just have to load the file once.
	*/
	void setFamily(const char* familyname);

	/**
	* Add Mark
	*	mark		int		the mark you need to increase/decrease according to its sign
	* You want to change the things
	*/
	void addMark(int mark);

	QRectF boundingRect() const;
	void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = Q_NULLPTR);
	void timerEvent(QTimerEvent* timer);

private:
	// marks
	int mark;
	int new_mark;
	// font
	QFont font;
	QString family;
	qreal myOpacity;
	qreal opacityStep = 0.08;
	char buf[12];
	// bounding box
	QRectF rect;
	// timer and lock
	int myTimer;
	QMutex lock;
};