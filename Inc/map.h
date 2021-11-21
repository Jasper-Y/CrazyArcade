#ifndef MAP
#define MAP

#include "crazyarcade.h"

class Map {
  protected:
    char *pix = nullptr;

  public:
    Map();
    ~Map();
    void CleanUp();
    void Create(int w, int h);
    // void SetPixel(int x, int y, int p);
    GridStatus GetPixel(int x, int y) const;
    void SetGrid(int row, int col, GridStatus grid_status);
    GridStatus GetGrid(int row, int col);
    bool Reachable(int x, int y);
    void Draw(YsRawPngDecoder *png1) const;
};
#endif