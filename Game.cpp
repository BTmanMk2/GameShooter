#include "Game.h"

GameManager::GameManager(int width, int height, GameProtocol pro, QWidget * parent)
	:QWidget(parent)
{
	// resize the window
	this->resize(width, height);

	// set the view
	scene = new QGraphicsScene();
	scene->setSceneRect(0, 0, width, height);
	view = new QGraphicsView(scene, this);
	

	// add items to scene
	switch (pro)
	{
	case SINGLE_PLAYER:		// single player game
	{
		int loc_x = width / 2 / 2;
		// load the bgmanager
		bm = new BgManager(BG_SKY, BG_SKY_HOLE, loc_x);
		scene->addItem(bm);
		// load the Fall Manager
		// load the water level
		waterLevel = new WaterLevel(WATERSVG, loc_x, 0);
		scene->addItem(waterLevel);
		// load the mark item
		mi = new MarkItem(FONT, 16, loc_x + 10, 10);
		scene->addItem(mi);
		// test
		// scene->addRect(0, 0, width, height);
	}
	break;
	case COUPLE_PLAYER:
	default:
		break;
	}
	qsrand(time(NULL));
}

GameManager::~GameManager()
{
	if (bm) delete bm;
	if (waterLevel) delete waterLevel;
	if (view) delete view;
	if (scene) delete scene;
}

MarkItem::MarkItem(const char * fontfile, int font_size, int x, int y, GameProtocol layer, QGraphicsItem * parent)
	:mark(0), QGraphicsItem(parent), rect(0, 0, 100, 40), myOpacity(1), new_mark(-1)
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
		font.setPointSize(font_size);
	}
}

void MarkItem::setFamily(const char * familyname)
{

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
	painter->drawText(rect, itoa(mark, buf, 10));
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
