#include "crazyarcade.h"
#include "map.h"
#include <queue>

class Bubble {
  private:
    int x, y;
    int life_span;
    int time_counter;

  public:
    Bubble(Map *map);
    void SetLoc(int cur_x, int cur_y);
    int Update();
    void Explode();
    void Draw();
};

class BubbleManager {
  private:
    int capacity;
    int range;
    std::queue<Bubble *> bubble_list;
public:
    BubbleManager(Map *map);
    void GetProps(int prop_status);
    void UpdateBubbles();
    void LayBubble();
};