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
*	GameManager	-- WaterLevel	(Water & Sand)
*				-- BgManager	(Background)
*				-- MarkItem		(Score & Mark)
*				-- StoneManager	(Stones)
*
*/
#pragma once
#include"Protocol.h"
#include"WaterLevel.h"
#include"BgManager.h"
#include"SandLevel.h"
#include "stonemanager.h"
#include<QWidget>
#include<QGraphicsScene>
#include<QGraphicsView>
#include <QGraphicsItem>
#include<QFontDatabase>
#include<QFont>
#include<QLabel>
#include<sstream>
#include<QMutex>
#include<QFile>
#include<QTextStream>
#include<QMainWindow>
#include<QApplication>
#include <QObject>

//#include "fragment.h"

#ifdef _OTHER_MANAGER
class FallManager;
#endif

class MarkItem;
class GameManager : public QWidget, QObject
{
	Q_OBJECT
public:
	
	/**
	* Constructor
	*	width	int	:the width of the game
	*	height	int	:the height of the game
	*	pro	GamePro	:the game mode
	*	parent	QWi	:the parent widget of the game
	* Build up the whole system.
	*/
	GameManager(QMainWindow* main, int width, int height, GameProtocol pro = SINGLE_PLAYER, QWidget* parent = nullptr);
	~GameManager();
	
	/********************* Call Back **********************/
	/**
	* Call Back
	*	player	GaP	:which player respond to that
	*	data	int	:data to interact
	* This Part defines a general function that needed to be
	* used during the game. Instead of using event, we use call
	* back functions to get instant effect.
	*/
	void hitOne(GameProtocol& player, int score);
	void hitDiamond(GameProtocol& player, int height, int score);	// height is below zero
	void missOne(GameProtocol& player, int height);
	int getCurrentScore(GameProtocol& player) const;
	int getCurrentY(GameProtocol& player) const;

	/*********************** Events **************************/
	void keyPressEvent(QKeyEvent* event);

	

private:
	/********************* Managers ***********************/
#ifdef _OTHER_MANAGER
	FallManager* fm;
	SandLevel* sandLevel;
#endif
	MarkItem* mi1;
	BgManager* bm1;
	WaterLevel* waterLevel;

	MarkItem* mi2;
	BgManager* bm2;
	SandLevel* sandLevel;

	StoneManager* sm1;
	StoneManager* sm2;

	//fragment *frag;
	/********************* Render ************************/
	QGraphicsScene* scene;
	QGraphicsView* view;
	QMainWindow* window;

	/********************* Element ***********************/
	GameProtocol gameMode;				// game mode, SINGLE or COUPLE
	int player1_score, player2_score;	// the score of the game
	int player1_height, player2_height;	// the current water&sand height
	int player1_miss, player2_miss;		// the miss counter
	int player1_hit, player2_hit;		// the hit counter
	QMutex lock1, lock2;
	int safe_height;			// the highest hight you can get
	int highest_score;			// the highest score in the history
	static const int init_y = 540;
	void gameover();
	void single_over();
	void couple_over();

	void single_reset();
	void couple_reset();

	void countdown();
	//void gameover();

	//gun timer
	QTimer* gunTimer;

	public slots:
	void gunUpdate();
	
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
	MarkItem(const char* fontfile, int font_size, int x, int y, int flag = Qt::AlignLeft, GameProtocol layer = LAYER_WATER, QGraphicsItem* parent = Q_NULLPTR);

	/**
	* Set Family
	*	familyname	char*	if you didn't specify the fontfile in constructor(NULL), you can set the family name.
	* One use this function when the constructor's fontfile set to NULL, and you can set the font through this way,
	* especially when you have multi-MarkItem, then you just have to load the file once.
	*/
	void setFamily(QString& familyname);
	QString getFamily();
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
	int myFlag;
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