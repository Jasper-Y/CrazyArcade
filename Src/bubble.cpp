#include <stdio.h>
#include <stdlib.h>
#include <queue>
#include <bubble.h>

class Bubble 
{
private:
	int x, y;  // location of bubble
	int life_span;
    int time_counter;

public:
    Bubble();
    void SetLoc(int cur_x, int cur_y);
    void Explode(Map *map, int range);
    int Update(Map *map, int range);
    void Draw();
};

Bubble::Bubble()
{
    life_span = 20;
    time_counter = 0;
}

void Bubble::SetLoc(int cur_x, int cur_y)
{
    x = cur_x;
    y = cur_y;
}

void Bubble::Explode(Map *map, int range) 
{
    for (int i = 1; i <= range; i++) 
    {   
        int nx, ny;
        nx = i + x;
        ny = y;
        int grid_status = map.GetGrid(nx, ny);
        
        if (grid_status == GridInvalid || grid_status == GridIndestructible)
        {
            break;
        } 
        else if grid_status == GridFree) 
        {
            continue;
        } 
        else 
        {
            // randomly transfer explode obstacle to free/props
            srand(time(nullptr));
            status = rand() % 10;
            if (status <= 6) {
                map.SetGrid(nx, ny, GridFree);
            } 
            else if (status == 7)
            {
                map.SetGrid(nx, ny, GridMoreBubble);
            } 
            else if (status == 8)
            {
                map.SetGrid(nx, ny, GridLongerBubble);
            } 
            else if (status == 9) 
            {
                map.SetGrid(nx, ny, GridSpeedUp);
            }
        }
    }
    for (int i = -1; i >= -range; i--) {
        int nx, ny;
        nx = i + x;
        ny = y;
        int grid_status = map.GetGrid(nx, ny);

        if (grid_status == GridInvalid || grid_status == GridIndestructible) {
            break;
        } else if grid_status == GridFree) 
        {
                continue;
            }
        else {
            // randomly transfer explode obstacle to free/props
            srand(time(nullptr));
            status = rand() % 10;
            if (status <= 6) {
                map.SetGrid(nx, ny, GridFree);
            } else if (status == 7) {
                map.SetGrid(nx, ny, GridMoreBubble);
            } else if (status == 8) {
                map.SetGrid(nx, ny, GridLongerBubble);
            } else if (status == 9) {
                map.SetGrid(nx, ny, GridSpeedUp);
            }
        }
    }
    for (int i = 1; i <= range; i++) {
        int nx, ny;
        nx = x;
        ny = y + i;
        int grid_status = map.GetGrid(nx, ny);

        if (grid_status == GridInvalid || grid_status == GridIndestructible) {
            break;
        } else if grid_status == GridFree) 
        {
                continue;
            }
        else {
            // randomly transfer explode obstacle to free/props
            srand(time(nullptr));
            status = rand() % 10;
            if (status <= 6) {
                map.SetGrid(nx, ny, GridFree);
            } else if (status == 7) {
                map.SetGrid(nx, ny, GridMoreBubble);
            } else if (status == 8) {
                map.SetGrid(nx, ny, GridLongerBubble);
            } else if (status == 9) {
                map.SetGrid(nx, ny, GridSpeedUp);
            }
        }
    }
    for (int i = -1; i >= -range; i--) {
        int nx, ny;
        nx = x;
        ny = y + i;
        int grid_status = map.Get(nx, ny);

        if (grid_status == GridInvalid || grid_status == GridIndestructible) {
            break;
        } else if grid_status == GridFree) 
        {
                continue;
            }
        else {
            // randomly transfer explode obstacle to free/props
            srand(time(nullptr));
            status = rand() % 10;
            if (status <= 6) {
                map.Set(nx, ny, GridFree);
            } else if (status == 7) {
                map.Set(nx, ny, GridMoreBubble);
            } else if (status == 8) {
                map.Set(nx, ny, GridLongerBubble);
            } else if (status == 9) {
                map.Set(nx, ny, GridSpeedUp);
            }
        }
    }
    return;
}


int Bubble::Update(Map *map, int range) 
{   
    time_counter++;
    if (time_counter >= life_span) 
    {   
        Explode(Map * map, int range);
        return 1;
    }
    return 0;
}

void Bubble::Draw()
{
    return;
}


class BubbleManager() 
{
private:
    int capacity;
    int range;
    queue <Bubble> bubble_list;
    Map *map;

public:
    BubbleManager();
    ~BubbleManager();
    void GetProps(int prop_status);
    void UpdateBubbles(void);
    void LayBubble(int x, int y);
    void Explode(void);
}

BubbleManager::BubbleManager() 
{
    capacity = 1;
    range = 2;
    map = NULL;
}

BubbleManager::~BubbleManager() 
{
    if (map != NULL)
    {
        map = NULL;
    }
    capacity = 0;
    range = 0;
}


void BubbleManager::GetProps(int prop_status) 
{
    if (prop_status == GridMoreBubble) 
    {
        capacity++;
    } else if (prop_status == GridLongerBubble) {
        range++;
    }
}

void BubbleManager::UpdateBubbles(void) {
    size = bubble_list.size();
    for (int i = 0; i < size; i++)
    {
        bubble = bubble_list[i];
        status = bubble.Update(map, range);
        if (status == 1)
        {   
            bubble_list.pop();
        }
    }
}

void BubbleManager::LayBubble(int x, int y) 
{
    
    if (bubble_list.size() >= capacity) {
        return;
    }
    else {
        Bubble bubble;
        bubble.SetLoc(x, y);
        bubble_list.push(bubble);
    }
    return;
}   

