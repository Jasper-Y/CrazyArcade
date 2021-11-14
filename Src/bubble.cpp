#include <stdio.h>
#include <stdlib.h>
#include <queue>

class Bubble 
{
private:
	int x, y;  // location of bubble
	int life_span;
    int time_counter;

public:
    Bubble();
    void SetLoc(int cur_x, int cur_y);
    int Update(int **grid);
	void Explode(int **grid);
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

void Bubble::Explode(int **grid, int map_size, int range) 
{
    int status = 0;
   
    for (int i = 1; i <= range; i++) 
    {   
        if (grid[i + x][y] == 2 || i + x == map_size - 1)
        {
            break;
        }
        else if(grid[i + x][y] == 0) 
        {
            continue;
        } else {
            // randomly transfer explode obstacle to free/props
            srand(time(nullptr));
            status = rand() % 10;
            if (status <= 6) {
                grid[i + x][y] = 0;
            } 
            else if (status == 7)
            {
                grid[i + x][y] = 3;
            } 
            else if (status == 8)
            {
                grid[i + x][y] = 4;
            } 
            else if (status == 9) 
            {
                grid[i + x][y] = 5;
            }
        }
    }
    for (int i = -1; i >= -range; i--) {
        if (grid[i + x][y] == 2 || i + x == 0) {
            break;
        } else if (grid[i + x][y] == 0) {
            continue;
        } else {
            // randomly transfer explode obstacle to free/props
            srand(time(nullptr));
            status = rand() % 10;
            if (status <= 6) {
                grid[i + x][y] = 0;
            } else if (status == 7) {
                grid[i + x][y] = 3;
            } else if (status == 8) {
                grid[i + x][y] = 4;
            } else if (status == 9) {
                grid[i + x][y] = 5;
            }
        }
    }
    for (int i = 1; i <= range; i++) {
        if (grid[x][i + y] == 2 || i + y == map_size - 1) {
            break;
        } else if (grid[x][i + y] == 0) {
            continue;
        } else {
            // randomly transfer explode obstacle to free/props
            srand(time(nullptr));
            status = rand() % 10;
            if (status <= 6) {
                grid[x][i + y] = 0;
            } else if (status == 7) {
                grid[x][i + y] = 3;
            } else if (status == 8) {
                grid[x][i + y] = 4;
            } else if (status == 9) {
                grid[x][i + y] = 5;
            }
        }
    }
    for (int i = -1; i >= -range; i--) {
        if (grid[x][i + y] == 2 || i + y == 0) {
            break;
        } else if (grid[x][i + y] == 0) {
            continue;
        } else {
            // randomly transfer explode obstacle to free/props
            srand(time(nullptr));
            status = rand() % 10;
            if (status <= 6) {
                grid[x][i + y] = 0;
            } else if (status == 7) {
                grid[x][i + y] = 3;
            } else if (status == 8) {
                grid[x][i + y] = 4;
            } else if (status == 9) {
                grid[x][i + y] = 5;
            }
        }
    }

    return;
}


int Bubble::Update(int **grid, int map_size, int range)
{   
    time_counter++;
    if (time_counter >= life_span) 
    {
        Explode(grid, map_size, range);
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

public:
    BubbleManager();
    void GetProps(int prop_status);
    void UpdateBubbles(int **grid, int map_size, int range);
    void LayBubble(int x, int y);
}

BubbleManager::BubbleManager() 
{
    capacity = 1;
    range = 2;
}

void BubbleManager::GetProps(int prop_status) 
{
    if (prop_status == 3) 
    {
        capacity++;
    } else if (prop_status == 4) {
        range++;
    }
}

void UpdateBubbles(int **grid, int map_size, int range) {
    size = bubble_list.size();
    for (int i = 0; i < size; i++)
    {
        bubble = bubble_list[i];
        status = bubble.Update(grid, map_size, range);
        if (status == 1)
        {
            bubble_list.pop();
        }
    }
}

void LayBubble(int x, int y)
{
    Bubble bubble;
    bubble.SetLoc(x, y);

    if (bubble_list.size() >= capacity) {
        return;
    }
    else {
        bubble_list.push(bubble);
    }
    return;
}   

