#pragma once

#define WATERSVG	"model/a.svg"
#define BG_SKY		"img/bg-b.png"
#define BG_SKY_HOLE	"img/bg-b-1.png"
#define FONT		"font/DIN Alternate Bold.ttf"

enum GameProtocol
{
	PLACE_HOLDER = 0x1,
	/**************** Game Initial Protocol ****************/
	SINGLE_PLAYER,			// game start with single player
	COUPLE_PLAYER,			// game start with a couple

	/**************** Game Layer Protocol ******************/
	LAYER_HOLE,
	LAYER_FALL_B,
	LAYER_BG,
	LAYER_FALL_A,
	LAYER_WATER,
};