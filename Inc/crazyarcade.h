#ifndef BUBBLE
#define BUBBLE
#include "bubble.h"
#endif

#ifndef PLAYER
#define PLAYER
#include "player.h"
#endif

#ifndef MAP
#define MAP
#include "map.h"
#endif

#ifndef YS_FILES
#define YS_FILES
#include "fssimplewindow.h"
#include "ysglfontdata.h"
#include "yspng.h"
#include "yspngenc.h"
#include "yssimplesound.h"
#endif

#include <stdio.h>
#include <stdlib.h>

#define ROW 13
#define COLUMN 15
#define RESOLUTION 60

enum GridStatus {
    GridInvalid = 0,
    GridFree,
    GridDestructible,
    GridIndestructible,
    GridMoreBubble,
    GridLongerBubble,
    GridSpeedUp
};




