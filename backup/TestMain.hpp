#pragma once
#ifndef TESTMAIN_HPP
#define TESTMAIN_HPP
#include <QMainWindow>
#include <qgraphicsscene.h>
#include <qgraphicsview.h>
#include "ui_TestMain.h"

#include"Game.h"

#include <QtSvg\qgraphicssvgitem.h>

class TestMain : public QMainWindow, public Ui::TestMain {
	Q_OBJECT

public:
	TestMain(int agrc, char* argv[], QWidget * parent = Q_NULLPTR);
	~TestMain();

private:
	Ui::TestMain ui;
};

#endif // TESTMAIN_HPP