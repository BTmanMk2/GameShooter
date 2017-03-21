#pragma once

#define WATERSVG	"model/a1.svg"
#define SANDSVG		"model/des.svg"
#define BG_SKY		"img/bg-b.png"
#define BG_SKY_HOLE	"img/bg-b-1.png"
#define BG_SAND		"img/bg-a.png"
#define BG_SAND_HOLE "img/bg-a-1.png"
#define FONT		"font/DIN Alternate Bold.ttf"
#define SCORE_FILE	"GameShooterLog"
#define DES_COLOR_FILE "model/des0.clr"

#define STONE_PREFIX "img/stone-"
#define SMASH_FOLDER "-smash"
#define STONE_SUFFIX ".png"
#define STONE_SVG ".svg"

#define SOUND_TEST "sound/test.ogg"

enum GameProtocol
{
	PLACE_HOLDER = 0x0,
	/**************** Game Initial Protocol ****************/
	SINGLE_PLAYER,			// game start with single player
	COUPLE_PLAYER,			// game start with a couple
	PLAYER1,
	PLAYER2,

	/**************** Game Layer Protocol ******************/
	LAYER_HOLE,				// the hole where the stones drop
	//LAYER_BG,		// the backgroud layer
	//LAYER_TEMP=0x7f,
	LAYER_STONE_BACK = 0x7f,		// the stone layer
	LAYER_BG = 0x80,		// the backgroud layer
	LAYER_STONE_FRONT,		// the stone layer
	LAYER_WATER,			// the water/sand layer
	LAYER_SAND,				// the sand layer

	/**************** Stone Status Protocol ******************/
	STONE_BIRTH = 0x100,
	STONE_WAIT,
	STONE_ABOVE_LINE,
	STONE_UNDER_LINE,
	STONE_FRAG
};


const unsigned int stoneTypes = 12;

const double minSpeed = 2;
const double maxSpeed = 10;
const double speedStep = 0.2;

const int stoneBirthRangeX = 10;

const double stoneBirthScale = 0.7;
const double stoneMaxScale = 1.0;

const int levelThreshold = 50;

//value needed below
const int singleWindowWidth = 0;
const int singleWindowHeight = 0;

const int shootLineHeight = 200;

const int player1BirthOffsetX = 0;
const int player1BirthOffsetY = 0;
const int player2BirthOffsetX = 0;
const int player2BirthOffsetY = 0;

const int dropSampleRate = 0;
const int spawnSampleRate = 0;