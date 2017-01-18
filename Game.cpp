#include "Game.h"

GameManager::GameManager(int width, int height, GameProtocol pro, QWidget * parent)
	:QWidget(parent), bm2(nullptr), mi2(nullptr), waterLevel2(nullptr)
{
	// resize the window
	this->resize(width, height);

	// set the view
	scene = new QGraphicsScene();
	scene->setSceneRect(0, 0, width, height);
	view = new QGraphicsView(scene, this);

	// get the highest score
	highest_score = 0;
	QFile file(SCORE_FILE);
	if (file.open(QIODevice::ReadOnly))
	{
		QTextStream in(&file);
		in >> highest_score;
		file.close();
	}

	// add items to scene
	switch (pro)
	{
	case SINGLE_PLAYER:		// single player game
	{
		int loc_x = width / 2 / 2;
		// load the bgmanager
		bm1 = new BgManager(BG_SKY, BG_SKY_HOLE, loc_x);
		scene->addItem(bm1);
		// load the Fall Manager
		// load the water level
		waterLevel1 = new WaterLevel(WATERSVG, loc_x, 0);
		scene->addItem(waterLevel1);
		// load the mark item
		mi1 = new MarkItem(FONT, 16, loc_x + 10, 10);
		scene->addItem(mi1);
		// test
		// scene->addRect(0, 0, width, height);
	}
	break;
	case COUPLE_PLAYER:
	{
		// load the back ground
		bm1 = new BgManager(BG_SAND, BG_SAND_HOLE, 0);
		bm1->holePos(175, 31);
		bm2 = new BgManager(BG_SKY, BG_SKY_HOLE, 512);
		mi1 = new MarkItem(FONT, 16, 512 - 100, 10, Qt::AlignRight);
		mi2 = new MarkItem(nullptr, 16, 512 + 20, 10, Qt::AlignLeft);
		mi2->setFamily(mi1->getFamily());
		scene->addItem(bm1);
		scene->addItem(bm2);
		scene->addItem(mi1);
		scene->addItem(mi2);
		// load the water & sand level
		waterLevel1 = new WaterLevel(WATERSVG, 512, 0);
		waterLevel2 = new WaterLevel(WATERSVG, 0, 0);
		scene->addItem(waterLevel1);
		scene->addItem(waterLevel2);

	}
	break;
	default:
		break;
	}
	qsrand(time(NULL));
	mi1->addMark(10);
	mi2->addMark(10);
}

GameManager::~GameManager()
{
	if (bm1) delete bm1;
	if (waterLevel1) delete waterLevel1;
	if (bm2) delete bm2;
	if (waterLevel2) delete waterLevel2;
	if (view) delete view;
	if (scene) delete scene;
	QFile file(SCORE_FILE);
	file.open(QIODevice::WriteOnly);
	QTextStream out(&file);
	out << highest_score << endl;
	file.close();
}

void GameManager::hitOne(GameProtocol & player, int score)
{
}

void GameManager::missOne(GameProtocol & player, int score)
{
}

int GameManager::getCurrentScore(GameProtocol & player) const
{
	if (player == PLAYER1) return player1_score;
	else return player2_score;
}

void GameManager::over()
{
	if (gameMode == SINGLE_PLAYER) single_over();
	else couple_over();
}

void GameManager::single_over()
{
}

void GameManager::couple_over()
{
}

MarkItem::MarkItem(const char * fontfile, int font_size, int x, int y, int flag, GameProtocol layer, QGraphicsItem * parent)
	:mark(0), QGraphicsItem(parent), rect(0, 0, 80, 20), myOpacity(1), new_mark(-1), myFlag(flag)
{
	// init the location
	setPos(x, y);
	setZValue(layer);

	// build up the font from the file
	if (fontfile)
	{
		int fontId = QFontDatabase::addApplicationFont(fontfile);
		QStringList fontFamilies = QFontDatabase::applicationFontFamilies(fontId);
		font.setFamily(fontFamilies.at(0));
		family = fontFamilies.at(0);
		qDebug("Family Name: %s", family.toStdString().c_str());
	}
	font.setPointSize(font_size);
}

void MarkItem::setFamily(QString& familyname)
{
	family = familyname;
	font.setFamily(family);
	update();
}

QString MarkItem::getFamily()
{
	return family;
}

void MarkItem::addMark(int mark)
{
	if (mark == 0) return;
	lock.lock();
	if (new_mark == -1)
	{
		new_mark = this->mark + mark;
		if (new_mark < 0) new_mark = 0;
		myTimer = startTimer(20);
		qDebug("Timer started: %d", myTimer);
	}
	else if (new_mark >= 0)
	{
		new_mark += mark;
	}
	else
	{
		this->mark += mark;
	}
	lock.unlock();
}

QRectF MarkItem::boundingRect() const
{
	return rect;
}

void MarkItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
	painter->setFont(font);
	painter->setPen(Qt::white);
	painter->setOpacity(myOpacity);
	painter->drawText(rect, myFlag, itoa(mark, buf, 10));
}

void MarkItem::timerEvent(QTimerEvent * timer)
{
	// all done
	lock.lock();
	qDebug("We are In!!");
	if (new_mark == -1)
	{
		if (myTimer != 0)
		{
			killTimer(myTimer);
			myTimer = 0;
		}
		lock.unlock();
		update();
		return;
	}

	// new value emerging
	if (new_mark == -2)
	{
		myOpacity += opacityStep;
		if (myOpacity >= 1)
		{
			myOpacity = 1;
			new_mark = -1;
		}
		lock.unlock();
		update();
		return;
	}

	// old value disappearing
	myOpacity -= opacityStep;
	if (myOpacity <= 0)
	{
		myOpacity = 0;
		mark = new_mark;
		new_mark = -2;
	}
	lock.unlock();
	update();
}
