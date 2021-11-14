#include <iostream>
#include "map.h"
#include <stdio.h>
#include <vector>

void main()
{
  Map test_map;

  vector<vector<int>> input_vector;
  
  for(int i = 0; i < 10; i++)
  {
    for(int j = 0; j < 10; j++)
    {
      input_vector[i][j] = i % 6;
    }
  }

  test_map.InitializeMap(input_vector);
  vector<vector<int>> new_input_vector = test_map.GetMap();
  // print answer
  for(int i = 0; i < 10; i++)
  {
    for(int j = 0; j < 10; j++)
    {
      std::cout << new_input_vector[i][j] << std::endl;
      printf("end testing initialization map");
    }
  }
  test_map.SetGrid(0, 0, 5);
  std::cout << test_map.GetGrid(0, 0) << std::endl;
  printf("end testing set & get map");
}