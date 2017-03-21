#pragma once
#ifndef TESTMAIN_HPP
#define TESTMAIN_HPP
#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include "ui_TestMain.h"

#include"Game.h"

#include <QtSvg\QGraphicsSvgItem>

class TestMain : public QMainWindow, public Ui::TestMain {
	Q_OBJECT

public:
	TestMain(int agrc, char* argv[], QWidget * parent = Q_NULLPTR);
	~TestMain();

private:
	Ui::TestMain ui;
};

#endif // TESTMAIN_HPP