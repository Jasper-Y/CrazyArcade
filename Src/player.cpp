// for windows visual studio compiler
#define _CRT_SECURE_NO_WARNINGS
#include "player.h"
#include <string>

Player::Player() {
    x = -1; // use -1 as empty value
    y = -1;
    v = 100;
    player_id = 1;
    state = PlayerAlive;
    heading = PlayerUp;

    std::string player_id_str = std::string("p") + std::to_string(player_id);
    std::string temp;
    char char_arr[10];

    temp = player_id_str + std::string("back.png");
    strcpy(char_arr, temp.c_str());
    png[0].Decode(char_arr);
    temp = player_id_str + std::string("front.png");
    strcpy(char_arr, temp.c_str());
    png[1].Decode(char_arr);
    temp = player_id_str + std::string("left.png");
    strcpy(char_arr, temp.c_str());
    png[2].Decode(char_arr);
    temp = player_id_str + std::string("right.png");
    strcpy(char_arr, temp.c_str());
    png[3].Decode(char_arr);

    map = nullptr;
    bubble_manager = new BubbleManager(map);

}

void Player::SetLocation(int loc_x, int loc_y) {
    x = loc_x;
    y = loc_y;
}

Player::Player(int loc_x, int loc_y, int id, Bitmap *m) {
    x = loc_x;
    y = loc_y;
    v = 100;
    state = PlayerAlive;
    heading = PlayerUp;

    map = m;
    player_id = id;

    std::string player_id_str = std::string("p") + std::to_string(player_id);
    std::string temp;
    char char_arr[100];
    temp = player_id_str + std::string("front.png");
    strcpy(char_arr, temp.c_str());
    png[0].Decode(char_arr);
    png[0].Flip();

    temp = player_id_str + std::string("back.png");
    strcpy(char_arr, temp.c_str());
    png[1].Decode(char_arr);
    png[1].Flip();

    temp = player_id_str + std::string("left.png");
    strcpy(char_arr, temp.c_str());
    png[2].Decode(char_arr);
    png[2].Flip();

    temp = player_id_str + std::string("right.png");
    strcpy(char_arr, temp.c_str());
    png[3].Decode(char_arr);
    png[3].Flip();

    bubble_manager = new BubbleManager(map);
}

void Player::Draw() const {
    if (state == PlayerDead) {
        return;
    }
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
    // png[i].Flip();
    glDrawPixels(png[i].wid, png[i].hei, GL_RGBA, GL_UNSIGNED_BYTE,
                 png[i].rgba);
}

int Player::GetLocationX() const {
    return x;
}

int Player::GetLocationY() const {
    return y;
}

void Player::LayBubble() {
    // TODO: to be implemented
    bubble_manager->LayBubble((x + RESOLUTION / 2) / RESOLUTION,
                              (y - RESOLUTION / 2) / RESOLUTION);
}

PlayerState Player::Update() {
    bubble_manager->UpdateBubbles();
    GridStatus grid_state = map->GetGrid((x + RESOLUTION / 2) / RESOLUTION,
                                         (y - RESOLUTION / 2) / RESOLUTION);
    if (grid_state >= GridExplodingMid &&
        grid_state <= GridExplodingRightwards) {
        state = PlayerDead;
    } else if (grid_state == GridMoreBubble || grid_state == GridLongerBubble) {
        bubble_manager->AddProps(grid_state);
        map->SetGrid((x + RESOLUTION / 2) / RESOLUTION,
                     (y - RESOLUTION / 2) / RESOLUTION, GridFree);
    } else if (grid_state == GridSpeedUp) {
        v = v <= 150 ? v + 50 : v;
        map->SetGrid((x + RESOLUTION / 2) / RESOLUTION,
                     (y - RESOLUTION / 2) / RESOLUTION, GridFree);
    }
    return state;
}

void Player::MovePlayer(PlayerHeading h) {
    if (state == PlayerDead) {
        return;
    }
    heading = h;
    int next_y = y, next_x = x;
    int backup_y = y, backup_x = x;
    switch (h) {
    case PlayerUp: {
        // use ordinary coordinate system
        next_y = y - PLAYER_DT * v;
        if (map->Reachable(x + RESOLUTION / 2, next_y - RESOLUTION / 2)) {
            y = next_y;
        }
        break;
    }
    case PlayerDown: {
        next_y = y + PLAYER_DT * v;
        if (map->Reachable(x + RESOLUTION / 2, next_y)) {
            y = next_y;
        }
        break;
    }
    case PlayerRight: {
        next_x = x + PLAYER_DT * v;
        if (y % RESOLUTION <= RESOLUTION / 3 && y % RESOLUTION > 0) {
            if (!map->Reachable(next_x + RESOLUTION, y)) {
                break;
            }
        }
        if (map->Reachable(next_x + RESOLUTION, y - RESOLUTION / 2)) {
            x = next_x;
        }
        break;
    }
    case PlayerLeft: {
        next_x = x - PLAYER_DT * v;
        if (y % RESOLUTION <= RESOLUTION / 3 && y % RESOLUTION > 0) {
            if (!map->Reachable(next_x + RESOLUTION / 4, y)) {
                break;
            }
        }
        if (next_x < 0) {
            break;
        }
        if (map->Reachable(next_x + RESOLUTION / 4, y - RESOLUTION / 2)) {
            x = next_x;
        }
        break;
    }
    }
    GridStatus pre_state =
        map->GetGrid((backup_x + RESOLUTION / 2) / RESOLUTION,
                     (backup_y - RESOLUTION / 2) / RESOLUTION);
    GridStatus next_state = map->GetGrid((x + RESOLUTION / 2) / RESOLUTION,
                                         (y - RESOLUTION / 2) / RESOLUTION);
    if (pre_state != GridBubbleWaitBig && pre_state != GridBubbleWaitSmall &&
        (next_state == GridBubbleWaitSmall ||
         next_state == GridBubbleWaitBig)) {
        x = backup_x;
        y = backup_y;
    }
}

void Player::isDead() {
    state = PlayerDead;
}

// int main(void){
// 	Player player_1 = Player();
// 	Player player_2 = Player(2,3);
// 	return 0;
// }
