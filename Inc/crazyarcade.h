#ifndef CRAZY_HEADERS
#define CRAZY_HEADERS

#include <chrono>
#include <ctime>
#include <deque>
#include <iostream>
#include <math.h>
#include <mutex>
#include <queue>
#include <stdio.h>
#include <stdlib.h>
#include <thread>
#include <time.h>
#include <vector>

#define ROW 13
#define COLUMN 15
#define RESOLUTION 60

enum GridStatus {
    GridInvalid = 0,
    GridFree,
    GridIndestructible,
    GridDestructible,
    GridMoreBubble,
    GridLongerBubble,
    GridSpeedUp,
    GridBubbleWaitBig,
    GridBubbleWaitSmall,
    GridExplodingMid,
    GridExplodingUpwards,
    GridExplodingDownwards,
    GridExplodingLeftwards,
    GridExplodingRightwards
};

enum CommandType {
    INVALID = 20,
    USER_TERMINATE,
    P1_FORWARD,
    P1_BACK,
    P1_LEFT,
    P1_RIGHT,
    P1_PLACE,
    P2_FORWARD,
    P2_BACK,
    P2_LEFT,
    P2_RIGHT,
    P2_PLACE
};

#endif

#ifndef YS_FILES
#define YS_FILES
#include "fssimplewindow.h"
#include "ysglfontdata.h"
#include "yspng.h"
#include "yssimplesound.h"
#endif
