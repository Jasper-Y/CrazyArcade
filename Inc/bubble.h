#ifndef BUBBLE
#define BUBBLE

#include "crazyarcade.h"
#include "map.h"

#define EXPLODE_TIME 200
#define DISAPPEAR_TIME 250

enum BubbleDirection {
    BubbleMid,
    BubbleUpside,
    BubbleDownside,
    BubbleLeftside,
    BubbleRightside
};

class Bubble {

  private:
    int x, y;
    int time_counter = 0;
    int my_range = 1;
    Bitmap *map;
    bool ChangeSingleGrid(int target_x, int target_y,
                          BubbleDirection direction);

    // YsSoundPlayer boom_player;
    // YsSoundPlayer::SoundData boom_wav;

  public:
    Bubble(Bitmap *map_in, int range, int x_in, int y_in);
    int Update();
};

class BubbleManager {
  private:
    int capacity = 1;
    int range = 1;
    std::deque<Bubble *> bubble_list;
    Bitmap *map;
    // lay bubble sound effect and bubble explode sound effect
    YsSoundPlayer bubble_player;
    YsSoundPlayer::SoundData bubble_wav;


  public:
    BubbleManager(Bitmap *map);
    ~BubbleManager();
    void AddProps(GridStatus prop_status);
    void UpdateBubbles();
    void LayBubble(int x, int y);
};
#endif