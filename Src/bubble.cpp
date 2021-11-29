#include "bubble.h"

Bubble::Bubble(Map *map) {
    this->map = map;
}

void Bubble::SetLoc(int cur_x, int cur_y) {
    x = cur_x;
    y = cur_y;
}

void Bubble::Explode(int range) {
    for (int i = 1; i <= range; i++) {
        int nx, ny;
        nx = i + x;
        ny = y;
        int grid_status = map->GetGrid(nx, ny);

        if (grid_status == GridInvalid || grid_status == GridIndestructible) {
            break;
        } else if (grid_status == GridFree) {
            continue;
        } else {
            // randomly transfer explode obstacle to free/props
            srand(time(nullptr));
            status = rand() % 10;
            if (status <= 6) {
                map->SetGrid(nx, ny, GridFree);
            } else if (status == 7) {
                map->SetGrid(nx, ny, GridMoreBubble);
            } else if (status == 8) {
                map->SetGrid(nx, ny, GridLongerBubble);
            } else if (status == 9) {
                map->SetGrid(nx, ny, GridSpeedUp);
            }
        }
    }
    for (int i = -1; i >= -range; i--) {
        int nx, ny;
        nx = i + x;
        ny = y;
        int grid_status = map->GetGrid(nx, ny);

        if (grid_status == GridInvalid || grid_status == GridIndestructible) {
            break;
        } else if (grid_status == GridFree) {
            continue;
        } else {
            // randomly transfer explode obstacle to free/props
            srand(time(nullptr));
            status = rand() % 10;
            if (status <= 6) {
                map->SetGrid(nx, ny, GridFree);
            } else if (status == 7) {
                map->SetGrid(nx, ny, GridMoreBubble);
            } else if (status == 8) {
                map->SetGrid(nx, ny, GridLongerBubble);
            } else if (status == 9) {
                map->SetGrid(nx, ny, GridSpeedUp);
            }
        }
    }
    for (int i = 1; i <= range; i++) {
        int nx, ny;
        nx = x;
        ny = y + i;
        int grid_status = map->GetGrid(nx, ny);

        if (grid_status == GridInvalid || grid_status == GridIndestructible) {
            break;
        } else if (grid_status == GridFree) {
            continue;
        } else {
            // randomly transfer explode obstacle to free/props
            srand(time(nullptr));
            status = rand() % 10;
            if (status <= 6) {
                map->SetGrid(nx, ny, GridFree);
            } else if (status == 7) {
                map->SetGrid(nx, ny, GridMoreBubble);
            } else if (status == 8) {
                map->SetGrid(nx, ny, GridLongerBubble);
            } else if (status == 9) {
                map->SetGrid(nx, ny, GridSpeedUp);
            }
        }
    }
    for (int i = -1; i >= -range; i--) {
        int nx, ny;
        nx = x;
        ny = y + i;
        int grid_status = map->Get(nx, ny);

        if (grid_status == GridInvalid || grid_status == GridIndestructible) {
            break;
        } else if (grid_status == GridFree) {
            continue;
        } else {
            // randomly transfer explode obstacle to free/props
            srand(time(nullptr));
            status = rand() % 10;
            if (status <= 6) {
                map->SetGrid(nx, ny, GridFree);
            } else if (status == 7) {
                map->SetGrid(nx, ny, GridMoreBubble);
            } else if (status == 8) {
                map->SetGrid(nx, ny, GridLongerBubble);
            } else if (status == 9) {
                map->SetGrid(nx, ny, GridSpeedUp);
            }
        }
    }
    return;
}

int Bubble::Update(int range) {
    time_counter++;
    if (time_counter >= life_span) {
        Explode(int range);
        return 1;
    }
    return 0;
}

void Bubble::Draw() {
    //TODO: implement draw
    return;
}

BubbleManager::BubbleManager(Map *map) {
    this->map = map;
}

BubbleManager::~BubbleManager() {
    for (std::queue<Bubble *>::iterator p = bubble_list.begin();
         p != bubble_list.end(); p++) {
        delete p;
    }
}

void BubbleManager::GetProps(int prop_status) {
    if (prop_status == GridMoreBubble) {
        capacity++;
    } else if (prop_status == GridLongerBubble) {
        range++;
    }
}

void BubbleManager::UpdateBubbles(void) {
    size = bubble_list.size();
    for (std::queue<Bubble *>::iterator it = bubble_list.begin();
         it != bubble_list.end(); it++) {
        int cnt = 0;
        if (it->Update(range) == 1) {
            cnt++;
        }
    }
    Bubble *p;
    while (cnt-- > 0) {
        p = bubble_list.pop();
        delete p;
    }
}

void BubbleManager::LayBubble(int x, int y) {
    if (bubble_list.size() >= capacity) {
        return;
    } else {
        Bubble *new_bubble = new Bubble(map);
        new_bubble->SetLoc(x, y);
        bubble_list.push(new_bubble);
    }
}
