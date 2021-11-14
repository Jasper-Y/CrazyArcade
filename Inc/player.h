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
	up,
	down,
	right,
	left
}
#endif

// #ifndef BUBBLE_MANAGER
// #define BUBBLE_MANAGER
// #include "bubbleManager.h"

// #endif

class Player{
private:
	int x, y; // location of player
	double v; // velocity of player
	enum Player_State state; // state of player
	enum Player_Heading heading; // heading direction of player
	BubbleManager bubble_manager;
public:
	Player(int loc_x, int loc_y);
	Player();
	// ~Player();
	int GetLocationX() const;
	int GetLocationY() const;

	// void GetProps(Prop prop);
	// void DrawPlayer() const;
	// void LayBubble();
	void MovePlayer(Player_Heading h);
	void isDead();
	void SetLocation(int loc_x, int loc_y);
}