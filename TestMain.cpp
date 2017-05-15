#include "TestMain.hpp"
#include "Protocol.h"
#include "gun.h"
#include <QMessageBox>

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

	this->hide();
	
	//debug
	if (dbg) {
		processCommand("gameOnline");
	}

	//setup gunwav
	for (int i = 0; i < GUNSHOT_CHANNELS; i++) {
		QMediaPlayer *tempGun = new QMediaPlayer();
		tempGun->setMedia(QUrl::fromLocalFile(GUN_SHOT));
		gunwav.push_back(tempGun);
	}

	if (dbg == false) {
		udpSocket = new QUdpSocket(this);
		udpSocket->bind(QHostAddress::Any, 8888);
		connect(udpSocket, SIGNAL(readyRead()), this, SLOT(processPendingDatagram()));
	}
	
	timer = new QTimer(this);
	QObject::connect(timer, SIGNAL(timeout()), this, SLOT(timeLimit()));
	timer->start(5000);

}

TestMain::~TestMain() {
	blueToothEnd();
	cameraEnd();
}

/*
*红外枪点检测//这里需要每帧调用，需要添加
*/
QPointF TestMain::infraredCheck()
{
	QPoint ret;
	if (GetSingleShootPointsMsg != NULL)
	{
		StPointsMsg *stpoints = GetSingleShootPointsMsg();
		while (stpoints != NULL)
		{
			//这里把枪点转换成鼠标点击事件，需要添加
			qDebug() << stpoints->stPointMsg.stPoint.x << stpoints->stPointMsg.stPoint.y;
			ret = QPoint(stpoints->stPointMsg.stPoint.x, stpoints->stPointMsg.stPoint.y);
			QMouseEvent* mouseEvent = new QMouseEvent(QMouseEvent::MouseButtonPress, QPoint(0, 0), ret, ret, Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
			QApplication::postEvent(this->childAt(ret), mouseEvent);

			//gunshot sound
			gunwav[channelId]->play();
			channelId++;
			if (channelId >= 7) {
				channelId = 0;
			}

			stpoints = stpoints->nextPointMsg;
		}
	}
	return ret;
}

void TestMain::timerEvent(QTimerEvent * event)
{
	infraredCheck();
	if (setter)
	{
		setter = false;
		killTimer(timerId);
		timerId = startTimer(0);
		return;
	}
	return;
}

void TestMain::processPendingDatagram()
{
	//等待数据接收完毕再做处理
	while (udpSocket->hasPendingDatagrams())
	{
		QByteArray recvData;
		recvData.resize(udpSocket->pendingDatagramSize());
		udpSocket->readDatagram(recvData.data(), recvData.size(), &clientIp, &clientPort); 
		//从发送方的包中读取数据以及ip和port并赋值给类的变量
		//statusText+="connet from "+clientIp.toString()+":"+QString::number(clientPort)+" ";
		//statusText+=recvData+"\n";
		//显示到状态标签
		QString msg(QString::fromLocal8Bit(recvData));
		qDebug() << "connet from " << clientIp.toString() << ":" << clientPort << " " << msg;
		//处理
		// ...
		processCommand(msg);

	}
}

void TestMain::processCommand(QString msg)
{
	if (msg == "gameOnline")
	{
		if (errorOccurs || onLineSignal) return;

		if (dbg == false)
			socketSend("gameOnline", clientIp, clientPort);

		// start the equipment
		if (!readGunXMl() ||
			!blueToothInitial() ||
			!cameraInitial())
		{
			//socketSend("gameError#0111111", clientIp, clientPort);
			errorOccurs = true;
			return;
		}
		equipmentTest = true;

		// start the game
		GameManager* gm = new GameManager(this, 1024, 768, gp);
		setCentralWidget(gm);
		if (dbg == false)
			socketSend("gameError#1111111", clientIp, clientPort);
		timerId = startTimer(1000);
		onLineSignal = true;
		this->showFullScreen();
	}
	else if (msg == "gameControl#quit")
	{
		QApplication::quit();
	}

}

//发送
void TestMain::socketSend(QString sendStr, QHostAddress targetIp, quint16 targetPort)
{
	udpSocket->writeDatagram(sendStr.toStdString().c_str(), sendStr.length(), targetIp, targetPort);
}

void TestMain::timeLimit()
{
	timer->stop();
	QObject::disconnect(timer, SIGNAL(timeout()), this, SLOT(timeLimit()));
	delete timer;
	
	if (!onLineSignal)
	{
		QApplication::quit();
	}
}

void TestMain::keyPressEvent(QKeyEvent* event)
{
	QMessageBox::information(this, "a", "event get");
	//exit message sent needed
}
