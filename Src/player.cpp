#include "player.h"
Player::Player() {
	x = -1; // use -1 as empty value
	y = -1;
	v = 0;
	state = alive;
	heading = up;
	// bubble_manager = BubbleManager();
}

void Player::SetLocation(int loc_x, int loc_y){
	x = loc_x;
	y = loc_y;
}

Player::Player(int loc_x, int loc_y) {
	x = loc_x;
	y = loc_y;
	v = 0;
	state = alive;
	heading = up;
	// bubble_manager = BubbleManager();
}

// Player::~Player(){

// }

int Player::GetLocationX() const {
	return x;
}

int Player::GetLocationY() const {
	return y;
}

// void Player::GetProps(Prop prop) {
// 	// TODO: to be implemented
// }

// void Player::DrawPlayer() const {
// 	//TODO: to be implemented
// }

// void Player::LayBubble() {
// 	//TODO: to be implemented
// 	bubble_manager.LayBubble(x, y);
// }

void Player::MovePlayer(Player_Heading h) {
	heading = h;
	switch(h) {
		case up:
			// use ordinary coordinate system
			y += PLAYER_DT * v;
			break;
		case down:
			y -= PLAYER_DT * v;
			break;
		case right:
			x += PLAYER_DT * v;
			break;
		case left:
			x -= PLAYER_DT * v;
			break;
	}
}

void Player::isDead(){
	state = dead;
}


int main(void){
	Player player_1 = Player();
	Player player_2 = Player(2,3);
	return 0;
}







