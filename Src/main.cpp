// #include "crazyarcade.h"
#include "fssimplewindow.h" // for test
#include <chrono>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>

const int X_WINDOW_RANGE = 1000;
const int Y_WINDOW_RANGE = 800;
enum CommandType {
    INVALID = 0,
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

class GameManager {
  private:
    bool is_playing = true;

  public:
    inline bool IsPlaying(void) {
        return is_playing;
    }
    inline void SetPlaying(bool state) {
        is_playing = state;
    }
    // @TODO: Add player, map, and other objects
    std::queue<CommandType> cmd_buf;
    std::mutex buf_mutex;
};

void ExecuteCommand(GameManager *manager) {
    CommandType cur_cmd;
    manager->SetPlaying(true);
    while (manager->IsPlaying()) {
        while (!manager->cmd_buf.empty()) {
            std::unique_lock<std::mutex> lock(manager->buf_mutex);
            cur_cmd = manager->cmd_buf.front();
            manager->cmd_buf.pop();
            lock.unlock();

            switch (cur_cmd) {
            case P1_FORWARD:
            case P1_BACK:
            case P1_LEFT:
            case P1_RIGHT: {
                printf("P1 movement: %d\n", cur_cmd);
                // @TODO: call player1 MovePlayer
                break;
            }
            case P1_PLACE: {
                printf("P1 place bubble: %d\n", cur_cmd);
                // @TODO: call player1 LayBubble
                break;
            }
            case P2_FORWARD:
            case P2_BACK:
            case P2_LEFT:
            case P2_RIGHT: {
                printf("P2 movement: %d\n", cur_cmd);
                // @TODO: call player2 MovePlayer
                break;
            }
            case P2_PLACE: {
                printf("P2 place bubble: %d\n", cur_cmd);
                // @TODO: call player2 LayBubble
                break;
            }
            default: {
                printf("[INFO] Stop playing\n");
                manager->SetPlaying(false);
                break;
            }
            }

            // @TODO: update state
            // manager->SetPlaying(some_return_state);
            if (!manager->IsPlaying()) {
                printf("[Game overrrr]\n");
                break;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

int main(void) {
    GameManager manager;
    std::thread background(&ExecuteCommand, &manager);
    FsOpenWindow(0, 0, X_WINDOW_RANGE, Y_WINDOW_RANGE, 1);
    int key, terminate = 0;
    CommandType command;
    while (terminate == 0) {
        command = INVALID;
        FsPollDevice();
        key = FsInkey();
        switch (key) {
        case FSKEY_ESC: {
            terminate = 1;
            command = USER_TERMINATE;
            printf("[INFO] Received esc\n");
            break;
        }
        case FSKEY_W:
            command = P1_FORWARD;
            break;
        case FSKEY_S:
            command = P1_BACK;
            break;
        case FSKEY_A:
            command = P1_LEFT;
            break;
        case FSKEY_D:
            command = P1_RIGHT;
            break;
        case FSKEY_UP:
            command = P2_FORWARD;
            break;
        case FSKEY_DOWN:
            command = P2_BACK;
            break;
        case FSKEY_LEFT:
            command = P2_LEFT;
            break;
        case FSKEY_RIGHT:
            command = P2_RIGHT;
            break;
        case FSKEY_SPACE: {
            command = P1_PLACE;
            break;
        }
        case FSKEY_ENTER: {
            command = P2_PLACE;
            break;
        }
        default:
            break;
        }

        if (command != INVALID) {
            std::lock_guard<std::mutex> lock(manager.buf_mutex);
            manager.cmd_buf.push(command);
        }

        if (!manager.IsPlaying()) {
            terminate = 1;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    // Wait background thread until finishes
    background.join();
    return 0;
}
