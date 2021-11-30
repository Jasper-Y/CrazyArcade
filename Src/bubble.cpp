#include "bubble.h"

Bubble::Bubble(Bitmap *map_in, int range, int x_in, int y_in)
    : map(map_in), my_range(range), x(x_in), y(y_in) {
    // Initialize to small, to make it big in first update
    map->SetGrid(x, y, GridBubbleWaitSmall);
}

// Return true if it's affected by neighbors, so re-update once
bool Bubble::ChangeSingleGrid(int target_x, int target_y,
                              BubbleDirection direction) {
    GridStatus pre_status = map->GetGrid(target_x, target_y);
    switch (pre_status) {
    case GridFree: {
        // Entering the exploding status or the exploding status is changed by
        // other bubbles (it is redundant to check the DISAPPEAR_TIME, just
        // leave it temporarily)
        if (time_counter < EXPLODE_TIME && direction == BubbleMid) {
            if (time_counter % 2 == 1) {
                map->SetGrid(x, y, GridBubbleWaitBig);
            } else {
                map->SetGrid(x, y, GridBubbleWaitSmall);
            }
        } else if (time_counter >= EXPLODE_TIME &&
                   time_counter < DISAPPEAR_TIME) {
            switch (direction) {
            case BubbleMid:
                map->SetGrid(target_x, target_y, GridExplodingMid);
                break;
            case BubbleUpside:
                map->SetGrid(target_x, target_y, GridExplodingUpwards);
                break;
            case BubbleDownside:
                map->SetGrid(target_x, target_y, GridExplodingDownwards);
                break;
            case BubbleLeftside:
                map->SetGrid(target_x, target_y, GridExplodingLeftwards);
                break;
            case BubbleRightside:
                map->SetGrid(target_x, target_y, GridExplodingRightwards);
                break;
            default:
                break;
            }
        }
        break;
    }
    case GridInvalid:
    case GridIndestructible:
    case GridMoreBubble:
    case GridLongerBubble:
    case GridSpeedUp:
        break;
    case GridDestructible: {
        // Generate props randomly
        if (time_counter >= EXPLODE_TIME) {
            int status = rand() % 10;
            switch (status) {
            case 7:
                map->SetGrid(target_x, target_y, GridMoreBubble);
                break;
            case 8:
                map->SetGrid(target_x, target_y, GridLongerBubble);
                break;
            case 9:
                map->SetGrid(target_x, target_y, GridSpeedUp);
                break;
            default:
                map->SetGrid(target_x, target_y, GridFree);
                break;
            }
        }
        break;
    }
    case GridBubbleWaitBig: {
        if (time_counter >= EXPLODE_TIME) {
            map->SetGrid(target_x, target_y, GridExplodingMid);
        } else {
            map->SetGrid(target_x, target_y, GridBubbleWaitSmall);
        }
        break;
    }
    case GridBubbleWaitSmall: {
        if (time_counter >= EXPLODE_TIME) {
            map->SetGrid(target_x, target_y, GridExplodingMid);
        } else {
            map->SetGrid(target_x, target_y, GridBubbleWaitBig);
        }
        break;
    }
    default: {
        // This includes all exploding status
        // If time_counter does not reach the explosion time but some grids are
        // exploding, it should be effected by neighbor bubbles.
        if (time_counter < EXPLODE_TIME) {
            time_counter = EXPLODE_TIME;
            return true;
        } else if (time_counter >= DISAPPEAR_TIME) {
            map->SetGrid(target_x, target_y, GridFree);
        }
        break;
    }
    }
    return false;
}

int Bubble::Update() {
    time_counter++;
    bool re_update = false;

    re_update = ChangeSingleGrid(x, y, BubbleMid);
    if (!re_update && time_counter < EXPLODE_TIME) {
        return time_counter;
    } else if (!re_update) {
        for (int i = -my_range; i <= my_range; i++) {
            if (i == 0) {
                continue;
            }
            if ((re_update = ChangeSingleGrid(x, y + i, BubbleDownside)) ==
                true) {
                break;
            }
            if ((re_update = ChangeSingleGrid(x + i, y, BubbleDownside)) ==
                true) {
                break;
            }
        }
    }
    if (re_update) {
        for (int i = -my_range; i <= my_range; i++) {
            if (i == 0) {
                continue;
            }
            ChangeSingleGrid(x, y + i, BubbleDownside);
            ChangeSingleGrid(x + i, y, BubbleDownside);
        }
    }
    return time_counter;
}

BubbleManager::BubbleManager(Bitmap *map) {
    this->map = map;
}

BubbleManager::~BubbleManager() {
    for (std::deque<Bubble *>::iterator p = bubble_list.begin();
         p != bubble_list.end(); p++) {
        delete *p;
    }
}

void BubbleManager::AddProps(GridStatus prop_status) {
    if (prop_status == GridMoreBubble) {
        capacity++;
    } else if (prop_status == GridLongerBubble) {
        range++;
    }
}

void BubbleManager::UpdateBubbles(void) {
    int size = bubble_list.size();
    int cnt = 0;
    for (std::deque<Bubble *>::iterator it = bubble_list.begin();
         it != bubble_list.end(); it++) {
        if ((*it)->Update() >= DISAPPEAR_TIME) {
            cnt++;
        }
    }
    Bubble *p;
    while (cnt-- > 0) {
        p = bubble_list.front();
        bubble_list.pop_front();
        delete p;
    }
}

void BubbleManager::LayBubble(int x, int y) {
    if (bubble_list.size() >= capacity) {
        return;
    }
    Bubble *new_bubble = new Bubble(map, range, x, y);
    bubble_list.push_back(new_bubble);
}
