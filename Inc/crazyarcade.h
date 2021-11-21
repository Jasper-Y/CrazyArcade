#ifndef YS_FILES
#define YS_FILES
#include "fssimplewindow.h"
#include "ysglfontdata.h"
#include "yspng.h"
#include "yspngenc.h"
#include "yssimplesound.h"
#endif

#include <chrono>
#include <ctime>
#include <iostream>
#include <math.h>
#include <mutex>
#include <stdio.h>
#include <stdlib.h>
#include <thread>
#include <time.h>

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

enum CommandType {
    INVALID = 10,
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
