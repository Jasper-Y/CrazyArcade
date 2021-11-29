#ifndef PLAYER_CLASS
#define PLAYER_CLASS
enum Player_State
{
	alive,
	dead,
	immortal
};

#define PLAYER_DT 0.1

enum Player_Heading
{
	PlayerUp,
	PlayerDown,
	PlayerRight,
	PlayerLeft
};

#include "map.h"
#endif

// #ifndef BUBBLE_MANAGER
// #define BUBBLE_MANAGER
// #include "bubbleManager.h"

// #endif

class Player{
private:
	int x, y; // location of player
	double v; // velocity of player
	Player_State state; // state of player
	Player_Heading heading; // heading direction of player
	// BubbleManager bubble_manager;
	Bitmap *map;
	int player_id; // for decoder
	YsRawPngDecoder png[4]; // to decode player image 0:front, 1:back 2:left 3:right
public:
	Player(int loc_x, int loc_y, int id, Bitmap *map);
	Player();
	// ~Player();
	int GetLocationX() const;
	int GetLocationY() const;

	// void GetProps(Prop prop);
	// void DrawPlayer() const;
	// void LayBubble();
	void Draw() const;
	void MovePlayer(Player_Heading h);
	void isDead();
	void SetLocation(int loc_x, int loc_y);
};