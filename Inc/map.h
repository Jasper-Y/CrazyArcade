#include "yspng.h"
#include "yspngenc.h"
#include "crazyarcade.h"

class Map
{
protected:
  char* pix = nullptr;

public:
  Map();
  ~Map();
  void CleanUp();
  void Create(int w, int h);
  //void SetPixel(int x, int y, int p);
  enum GetPixel(int x, int y) const;
  void SetGrid(int row, int col, enum grid_status);
  enum GetGrid(int row, int col);
  bool Reachable(int x, int y);
	void Draw(YsRawPngDecoder* png1) const;

};