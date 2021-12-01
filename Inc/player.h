#ifndef PLAYER_CLASS
#define PLAYER_CLASS

#define PLAYER_DT 0.1
#include "bubble.h"
#include "map.h"

enum PlayerState { PlayerAlive, PlayerDead, PlayerImmortal };

enum PlayerHeading { PlayerUp, PlayerDown, PlayerRight, PlayerLeft };

class Player {
  private:
    int x, y;              // location of player
    double v;              // velocity of player
    PlayerState state;     // state of player
    PlayerHeading heading; // heading direction of player
    BubbleManager *bubble_manager;
    Bitmap *map;
    int player_id; // for decoder
    YsRawPngDecoder
        png[4]; // to decode player image 0:front, 1:back 2:left 3:right

  public:
    Player(int loc_x, int loc_y, int id, Bitmap *map);
    Player();
    int GetLocationX() const;
    int GetLocationY() const;

    PlayerState Update();
    void LayBubble();
    void Draw() const;
    void MovePlayer(PlayerHeading h);
    void isDead();
    void SetLocation(int loc_x, int loc_y);
};

#endif