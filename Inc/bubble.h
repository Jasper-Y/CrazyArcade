#ifndef BUBBLE
#define BUBBLE

#include "crazyarcade.h"
#include "map.h"

class Bubble {
  private:
    int x, y;
    int life_span = 20;
    int time_counter = 0;
    Map *map;

  public:
    Bubble(Map *map);
    void SetLoc(int cur_x, int cur_y);
    int Update(int range);
    void Explode(int range);
    void Draw();
};

class BubbleManager {
  private:
    int capacity = 1;
    int range = 2;
    std::queue<Bubble *> bubble_list;
    Map *map;

  public:
    BubbleManager(Map *map);
    ~BubbleManager();
    void GetProps(int prop_status);
    void UpdateBubbles();
    void LayBubble(int x, int y);
};
#endif