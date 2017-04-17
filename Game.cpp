#include "Game.h"
#include <QKeyEvent>
#include "gun.h"

GameManager::GameManager(QMainWindow* main, int width, int height, GameProtocol pro, QWidget * parent)
	:QWidget(parent), bm2(nullptr), mi2(nullptr), sandLevel(nullptr), window(main),
	gameMode(pro), player1_score(0), player2_score(0), player1_hit(0), player2_hit(0),
	player1_height(0), player2_height(0), player1_miss(0), player2_miss(0)
{
	qsrand(time(NULL));
	// resize the window
	this->resize(width, height);

	// set the view
	scene = new QGraphicsScene();
	scene->setSceneRect(0, 0, width, height);
	//scene->setBackgroundBrush(QBrush(Qt::black));
	view = new QGraphicsView(scene, this);
	view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	view->setFixedSize(1024, 768);

	//add by wzy
	readGunXMl();
	cameraInitial();
	blueToothInitial();
	equipmentTest = true;

	// get the highest score
	highest_score = 0;
	QFile file(SCORE_FILE);
	if (file.open(QIODevice::ReadOnly))
	{
		QTextStream in(&file);
		in >> highest_score;
		file.close();
	}
	safe_height = 400;

	// add items to scene
	switch (pro)
	{
	case SINGLE_PLAYER:		// single player game
	{
		int loc_x = width / 2 / 2;
		// load the bgmanager
		bm1 = new BgManager(BG_SKY, BG_SKY_HOLE, scene, loc_x);
		scene->addItem(bm1);
		// load the Fall Manager
		// load the water level
		waterLevel = new WaterLevel(WATERSVG, loc_x, 0);
		scene->addItem(waterLevel);
		// load the mark item
		mi1 = new MarkItem(FONT, 16, loc_x + 10, 10);
		scene->addItem(mi1);
		// test
		QPointF poi1(loc_x, 0);
		sm1 = new StoneManager(poi1, PLAYER1, this);
		scene->addItem(sm1);
		sm1->setFlag(QGraphicsItem::ItemIsFocusable);
		sm1->setFocus();
	}
	break;
	case COUPLE_PLAYER:
	{
		// load the back ground
		bm1 = new BgManager(BG_SAND, BG_SAND_HOLE, scene, 0);
		bm1->holePos(175, 31);
		bm2 = new BgManager(BG_SKY, BG_SKY_HOLE, scene, 512);
		mi1 = new MarkItem(FONT, 16, 512 - 100, 10, Qt::AlignRight);
		mi2 = new MarkItem(nullptr, 16, 512 + 20, 10, Qt::AlignLeft);
		mi2->setFamily(mi1->getFamily());
		scene->addItem(bm1);
		scene->addItem(bm2);
		scene->addItem(mi1);
		scene->addItem(mi2);
		// load the water & sand level
		waterLevel = new WaterLevel(WATERSVG, 512, 0);
		sandLevel = new SandLevel(SANDSVG, 0, 0);
		scene->addItem(waterLevel);
		scene->addItem(sandLevel);

		QPointF poi1(0, 0);
		QPointF poi2(512, 0);
		sm1 = new StoneManager(poi1, PLAYER1, this);
		sm2 = new StoneManager(poi2, PLAYER2, this);
		scene->addItem(sm1);
		scene->addItem(sm2);
		sm1->addFrag();
		sm2->addFrag();
		sm1->setFlag(QGraphicsItem::ItemIsFocusable);
		sm2->setFlag(QGraphicsItem::ItemIsFocusable);
		sm1->setFocus();
		sm2->setFocus();

		//frag = new fragment(1, QPointF(20, 20));
		//scene->addItem(frag);

	}
	break;
	default:
		break;
	}

	

	mi1->addMark(10);

	gunTimer = new QTimer(this);
	QObject::connect(gunTimer, SIGNAL(timeout()), this, SLOT(gunUpdate()));
	gunTimer->start(0);
}

GameManager::~GameManager()
{
	if (bm1) delete bm1;
	if (waterLevel) delete waterLevel;
	if (bm2) delete bm2;
	if (sandLevel) delete sandLevel;
	if (view) delete view;
	if (scene) delete scene;
	QFile file(SCORE_FILE);
	file.open(QIODevice::WriteOnly);
	QTextStream out(&file);
	out << highest_score << endl;
	file.close();
	//add by wzy
	blueToothEnd();
	cameraEnd();
}

void GameManager::hitOne(GameProtocol & player, int score)
{
	if (player == PLAYER1)
	{
		lock1.lock();
		player1_score += score;
		player1_hit += 1;
		lock1.unlock();
		mi1->addMark(score);
	}
	else
	{
		lock2.lock();
		player2_score += score;
		player2_hit += 1;
		lock2.unlock();
		mi2->addMark(score);
	}
}

void GameManager::hitDiamond(GameProtocol & player, int height, int score)
{
	if (player == PLAYER1)
	{
		lock1.lock();
		player1_height += height;
		player1_score += score;
		player1_hit += 1;
		lock1.unlock();
		waterLevel->riseUp(-height);
	}
	else
	{
		lock2.lock();
		player2_height += height;
		player2_score += score;
		player2_hit += 1;
		lock2.unlock();
		sandLevel->riseUp(-height);
	}
}

void GameManager::missOne(GameProtocol & player, int height)
{
	if (player == PLAYER1)
	{
		lock1.lock();
		player1_height += height;
		player1_miss += 1;
		lock1.unlock();
		sandLevel->riseUp(-height);
		if (safe_height <= player1_height) over();
	}
	else
	{
		lock2.lock();
		player2_height += height;
		player2_miss += 1;
		lock2.unlock();
		waterLevel->riseUp(-height);
		if (safe_height <= player2_height) over();
	}
}

int GameManager::getCurrentScore(GameProtocol & player) const
{
	if (player == PLAYER1) return player1_score;
	else return player2_score;
}

int GameManager::getCurrentY(GameProtocol & player) const
{
	if (player == PLAYER1) return init_y - player1_height;
	else return init_y - player2_height;
}


void GameManager::over()
{
	if (gameMode == SINGLE_PLAYER) single_over();
	else couple_over();
}

void GameManager::single_over()
{
	sm1->stop();
	Sleep(5000);
	delete this;
	exit(0);
}

void GameManager::couple_over()
{
	sm1->stop();
	sm2->stop();
	Sleep(5000);
	delete this;
	exit(0);
}



void GameManager::gunUpdate()
{
	QPointF ret;
	if (GetSingleShootPointsMsg != NULL)
	{
		StPointsMsg *stpoints = GetSingleShootPointsMsg();
		while (stpoints != NULL)
		{
			//这里把枪点转换成鼠标点击事件，需要添加
			qDebug() << stpoints->stPointMsg.stPoint.x << stpoints->stPointMsg.stPoint.y;
			ret = QPointF(stpoints->stPointMsg.stPoint.x, stpoints->stPointMsg.stPoint.y);
			QMouseEvent *mouseEvent = new QMouseEvent(QMouseEvent::MouseButtonPress, QPointF(0, 0), QPointF(0, 0), QPoint(stpoints->stPointMsg.stPoint.x, stpoints->stPointMsg.stPoint.y), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
			QApplication::postEvent(view, mouseEvent);

			//QTest::mouseClick(this, Qt::LeftButton, 0, ret, -1);

			stpoints = stpoints->nextPointMsg;
		}
	}
	//return ret;
}

void GameManager::keyPressEvent(QKeyEvent* event)
{
	if (event->key() == Qt::Key_Escape) {
		delete this;
		exit(0);
	}
}

MarkItem::MarkItem(const char * fontfile, int font_size, int x, int y, int flag, GameProtocol layer, QGraphicsItem * parent)
	:mark(0), QGraphicsItem(parent), rect(0, 0, 80, 40), myOpacity(1), new_mark(-1), myFlag(flag)
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