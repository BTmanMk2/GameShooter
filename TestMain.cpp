#include "TestMain.hpp"
#include "Protocol.h"

TestMain::TestMain(int argc, char* argv[], QWidget * parent) : QMainWindow(parent){
	// analysis the arguments
	GameProtocol gp = SINGLE_PLAYER;
	if (argc == 2)
	{
		if (std::string(argv[1]) == "-couple")
		{
			gp = COUPLE_PLAYER;
		}
	}

	// set up
	ui.setupUi(this);
	this->setWindowTitle(tr("test"));
	this->resize(1024, 768);
	QPalette palette = this->palette();
	palette.setColor(QPalette::Window, QColor(255, 255, 255));
	this->setPalette(palette);

	// start the game
	GameManager* gm = new GameManager(1024, 768, gp);
	setCentralWidget(gm);
}

TestMain::~TestMain() {
	
}
