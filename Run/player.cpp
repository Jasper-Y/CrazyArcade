#include "player.h"
#include <string>


Player::Player() {
	x = -1; // use -1 as empty value
	y = -1;
	v = 0;
	player_id = 1;
	state = alive;
	heading = PlayerUp;

	std::string player_id_str = std::string("p") + std::to_string(player_id);
	std::string temp;
	char char_arr[10];

	temp = player_id_str + std::string("front.png");
	strcpy(char_arr, temp.c_str());
	png[0].Decode(char_arr);
	temp = player_id_str + std::string("back.png");
	strcpy(char_arr, temp.c_str());
	png[1].Decode(char_arr);
	temp = player_id_str + std::string("left.png");
	strcpy(char_arr, temp.c_str());
	png[2].Decode(char_arr);
	temp = player_id_str + std::string("right.png");
	strcpy(char_arr, temp.c_str());
	png[3].Decode(char_arr);

	map = nullptr;
	// bubble_manager = BubbleManager();
}

void Player::SetLocation(int loc_x, int loc_y){
	x = loc_x;
	y = loc_y;
}

Player::Player(int loc_x, int loc_y, int id, Bitmap *m) {
	x = loc_x;
	y = loc_y;
	v = 0;
	state = alive;
	heading = PlayerUp;

	map = m;
	player_id = id;

	std::string player_id_str = std::string("p") + std::to_string(player_id);
	std::string temp;
	char char_arr[100];
	temp = player_id_str + std::string("front.png");
	strcpy(char_arr, temp.c_str());
	png[0].Decode(char_arr);
	temp = player_id_str + std::string("back.png");
	strcpy(char_arr, temp.c_str());
	png[1].Decode(char_arr);
	temp = player_id_str + std::string("left.png");
	strcpy(char_arr, temp.c_str());
	png[2].Decode(char_arr);
	temp = player_id_str + std::string("right.png");
	strcpy(char_arr, temp.c_str());
	png[3].Decode(char_arr);


	// bubble_manager = BubbleManager();
}

void Player::Draw() const {
	glRasterPos2d(x, y);
	int i;
	switch (heading) {
		case PlayerDown:
			i = 0;
			break;
		case PlayerUp:
			i = 1;
			break;
		case PlayerLeft:
			i = 2;
			break;
		case PlayerRight:
			i = 3;
			break;
	}
	glDrawPixels(png[i].wid, png[i].hei, GL_RGBA, GL_UNSIGNED_BYTE,
                             png[i].rgba);

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
		case PlayerUp:
			// use ordinary coordinate system
			y += PLAYER_DT * v;
			break;
		case PlayerDown:
			y -= PLAYER_DT * v;
			break;
		case PlayerRight:
			x += PLAYER_DT * v;
			break;
		case PlayerLeft:
			x -= PLAYER_DT * v;
			break;
	}
}

void Player::isDead(){
	state = dead;
}


// int main(void){
// 	Player player_1 = Player();
// 	Player player_2 = Player(2,3);
// 	return 0;
// }







