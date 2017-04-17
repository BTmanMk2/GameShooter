#include "TestMain.hpp"
#include "Protocol.h"
//#include "gun.h"

TestMain::TestMain(int argc, char* argv[], QWidget * parent) : QMainWindow(parent){
	// analysis the arguments
	GameProtocol gp = COUPLE_PLAYER;
	if (argc == 2)
	{
		if (std::string(argv[1]) == "-couple")
		{
			gp = COUPLE_PLAYER;
		}
	}

	// set up
	ui.setupUi(this);
	ui.menuBar->hide();
	ui.statusBar->hide();
	ui.mainToolBar->hide();
	this->setWindowTitle(tr("test"));
	this->resize(1024, 768);
	QPalette palette = this->palette();
	palette.setColor(QPalette::Window, QColor(255, 255, 255));
	this->setPalette(palette);
	/*readGunXMl();
	blueToothInitial();
	cameraInitial();
	equipmentTest = true;*/

	

	// start the game
	GameManager* gm = new GameManager(this, 1024, 768, gp);
	setCentralWidget(gm);
	startTimer(20);
}

TestMain::~TestMain() {
	/*blueToothEnd();
	cameraEnd();*/
}

/*
*红外枪点检测//这里需要每帧调用，需要添加
*/
/*QPointF TestMain::infraredCheck()
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
			QMouseEvent mouseEvent(QMouseEvent::MouseButtonPress, QPoint(stpoints->stPointMsg.stPoint.x, stpoints->stPointMsg.stPoint.y), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
			QApplication::sendEvent(this, &mouseEvent);

			//QTest::mouseClick(this, Qt::LeftButton, 0, ret, -1);

			stpoints = stpoints->nextPointMsg;
		}
	}
	return ret;
}*/

void TestMain::timerEvent(QTimerEvent * event)
{
	//infraredCheck();
	return;
}
