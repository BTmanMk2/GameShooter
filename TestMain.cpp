#include "TestMain.hpp"
#include "Protocol.h"

TestMain::TestMain(int argc, char* argv[], QWidget * parent) : QMainWindow(parent) {
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
	ui.mainToolBar->hide();
	ui.statusBar->hide();

	this->setWindowTitle(tr("test"));
	this->resize(1024, 768);
	QPalette palette = this->palette();
	palette.setColor(QPalette::Window, QColor(0, 255, 0));
	this->setPalette(palette);

	// start the game
	GameManager* gm = new GameManager(this, 1024, 768, gp);
	setCentralWidget(gm);
}

TestMain::~TestMain() {
}