#pragma once

#define WATERSVG	"model/a.svg"
#define BG_SKY		"img/bg-b.png"
#define BG_SKY_HOLE	"img/bg-b-1.png"
#define BG_SAND		"img/bg-a.png"
#define BG_SAND_HOLE "img/bg-a-1.png"
#define FONT		"font/DIN Alternate Bold.ttf"
#define SCORE_FILE	"GameShooterLog"

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
	LAYER_STONE_BACK,		// the stone layer
	LAYER_BG = 0x80,		// the backgroud layer
	LAYER_STONE_FRONT,		// the stone layer
	LAYER_WATER,			// the water/sand layer

	/**************** Stone Status Protocol ******************/
	STONE_BIRTH = 0x100,
	STONE_ABOVE_LINE,
	STONE_UNDER_LINE,
};