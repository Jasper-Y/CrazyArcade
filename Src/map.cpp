#include <stdio.h>
#include <vector>
#include "map.h"

/*
Free = 0;
Destructible = 1;
Indestructible = 2;
MoreBubble = 3;
LongerBubble = 4;
SpeedUp = 5;
*/

void Map::InitializeMap(vector<vector<int>> input)
{
  map_size_row = input.size();
  map_size_col = input[0].size();

  for(int i = 0; i < map_size_row; i++)
  {
    for(int j = 0; j < map_size_col; j++)
    {
      map[i][j] = input[i][j];
    }
  }
}

vector<vector<int>> Map::GetMap()
{
  return map;
}

void Map::SetGrid(int x, int y, int value)
{
  map[x][y] = value;
}

int Map::GetGrid(int x, int y)
{
  return map[x][y];
}

void Map::DrawMap(); // JJJ