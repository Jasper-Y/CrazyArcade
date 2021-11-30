#ifndef MAP
#define MAP

#include "crazyarcade.h"

class Bitmap {
  private:
    int wid = 0, hei = 0;
    GridStatus *pix = nullptr;
    YsRawPngDecoder png[12];
    YsRawPngDecoder pngbackgroud;

  public:
    Bitmap();
    ~Bitmap();
    void CleanUp(void);
    void Create(int w, int h);
    void SetGrid(int x, int y, GridStatus p);

    // The return data type of GetGrid can be any integral data type.
    GridStatus GetGrid(int x, int y) const;
    void Drawbackgroud() const;
    void Draw() const;

    // @todo:
    bool Reachable(int x, int y) const;
    // void SetGrid(int row, int col, GridStatus grid_status);
    // GridStatus GetGrid(int row, int col) const;
};
#endif