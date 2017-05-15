#pragma once
#ifndef TESTMAIN_HPP
#define TESTMAIN_HPP
#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMouseEvent>
#include <QUdpSocket>
#include <QMediaPlayer>
#include "ui_TestMain.h"

#include <vector>

#include"Game.h"

#include <Qtsvg/QGraphicsSvgItem>

class TestMain : public QMainWindow, public Ui::TestMain {
	Q_OBJECT

public:
	TestMain(int agrc, char* argv[], QWidget * parent = Q_NULLPTR);
	~TestMain();
	void keyPressEvent(QKeyEvent* event);

	public slots:
	void timeLimit();
	void processPendingDatagram();

private:
	Ui::TestMain ui;
	QPointF TestMain::infraredCheck();
	bool setter = true;
	int timerId;
	GameProtocol gp = COUPLE_PLAYER;//SINGLE_PLAYER;

									// network rights
	void processCommand(QString msg);
	void socketSend(QString sendStr, QHostAddress targetIp, quint16 targetPort);
	bool errorOccurs = false;
	bool onLineSignal = false;
	QTimer* timer;
	QUdpSocket* udpSocket;
	QHostAddress clientIp;
	quint16 clientPort;

	//gunshot sound
	std::vector<QMediaPlayer*>gunwav;
	unsigned int channelId = 0;

protected:
	void timerEvent(QTimerEvent *event);
};

#endif // TESTMAIN_HPP