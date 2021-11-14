#include <stdio.h>
#include <vector>

/*
Free = 0;
Destructible = 1;
Indestructible = 2;
MoreBubble = 3;
LongerBubble = 4;
SpeedUp = 5;
*/

class Map
{
private:
  int map_size_row;
  int map_size_col;
  vector<vector<int>> map;

public:
  void InitializeMap(vector<vector<int>> input);
  vector<vector<int>> GetMap();
  void SetGrid(int x, int y, int value);
  int GetGrid(int x, int y);
  void DrawMap(); // JJJ
};