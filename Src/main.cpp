// clang-format off
#include "crazyarcade.h"
#include "bubble.h"
#include "map.h"
#include "player.h"
// clang-format on

class GameManager {
  private:
    bool is_playing = true;

  public:
    GameManager() {
        map = new Bitmap();
        p1 = new Player(100, 100, 1, map);
        p2 = new Player(800, 700, 2, map);
    }

    ~GameManager() {
        delete map;
        delete p1;
        delete p2;
    }

    inline bool IsPlaying(void) {
        return is_playing;
    }

    inline void SetPlaying(bool state) {
        is_playing = state;
    }

    /**
     * @brief Update the states and return the status of players
     *
     * @return false if the game is over
     */
    bool Update(void) {
        if (p1->Update() == PlayerDead) {
            is_playing = false;
            return false;
        }
        if (p2->Update() == PlayerDead) {
            is_playing = false;
            return false;
        }
        return true;
    }

    void Draw(void) {
        map->Draw();
        p1->Draw();
        p2->Draw();
    }

    Bitmap *map;
    Player *p1, *p2;
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
                manager->p1->MovePlayer(PlayerUp);
                break;
            case P1_BACK:
                manager->p1->MovePlayer(PlayerDown);
                break;
            case P1_LEFT:
                manager->p1->MovePlayer(PlayerLeft);
                break;
            case P1_RIGHT:
                manager->p1->MovePlayer(PlayerRight);
                break;
            case P1_PLACE: {
                manager->p1->LayBubble();
                break;
            }
            case P2_FORWARD:
                manager->p2->MovePlayer(PlayerUp);
                break;
            case P2_BACK:
                manager->p2->MovePlayer(PlayerDown);
                break;
            case P2_LEFT:
                manager->p2->MovePlayer(PlayerLeft);
                break;
            case P2_RIGHT:
                manager->p2->MovePlayer(PlayerRight);
                break;
            case P2_PLACE: {
                manager->p2->LayBubble();
                break;
            }
            default: {
                printf("[INFO] Stop playing\n");
                manager->SetPlaying(false);
                break;
            }
            }

            if (!manager->Update() || !manager->IsPlaying()) {
                printf("[Game overrrr]\n");
                break;
            }

            // Represent the operation cost for rendering
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        if (!manager->Update() || !manager->IsPlaying()) {
            printf("[Game overrrr]\n");
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    printf("[Quit thread...]\n");
}

void PushCommand(GameManager *manager, std::unique_lock<std::mutex> &my_lock,
                 CommandType cmd) {
    my_lock.lock();
    manager->cmd_buf.push(cmd);
    my_lock.unlock();
}

int main(void) {
    srand(time(nullptr));
    GameManager manager;
    FsOpenWindow(0, 0, RESOLUTION * COLUMN, RESOLUTION * ROW, 1);

    // bgm player
    YsSoundPlayer BGM_player;
    YsSoundPlayer::SoundData wav;
    char bgm_file_name[] = "bgm.wav";
    if (YSOK != wav.LoadWav(bgm_file_name)) {
        printf("failed to read %s\n", bgm_file_name);
    }else {
        BGM_player.Start();
        BGM_player.PlayBackground(wav);
    }

    std::thread background(&ExecuteCommand, &manager);
    int key, terminate = 0;
    CommandType command;
    while (terminate == 0) {
        std::unique_lock<std::mutex> lock(manager.buf_mutex);
        lock.unlock();
        FsPollDevice();
        if (FsGetKeyState(FSKEY_W) != 0) {
            PushCommand(&manager, lock, P1_FORWARD);
        }
        if (FsGetKeyState(FSKEY_S)) {
            PushCommand(&manager, lock, P1_BACK);
        }
        if (FsGetKeyState(FSKEY_A)) {
            PushCommand(&manager, lock, P1_LEFT);
        }
        if (FsGetKeyState(FSKEY_D)) {
            PushCommand(&manager, lock, P1_RIGHT);
        }
        if (FsGetKeyState(FSKEY_UP)) {
            PushCommand(&manager, lock, P2_FORWARD);
        }
        if (FsGetKeyState(FSKEY_DOWN)) {
            PushCommand(&manager, lock, P2_BACK);
        }
        if (FsGetKeyState(FSKEY_LEFT)) {
            PushCommand(&manager, lock, P2_LEFT);
        }
        if (FsGetKeyState(FSKEY_RIGHT)) {
            PushCommand(&manager, lock, P2_RIGHT);
        }
        if (FsGetKeyState(FSKEY_SPACE)) {
            PushCommand(&manager, lock, P1_PLACE);
        }
        if (FsGetKeyState(FSKEY_ENTER)) {
            PushCommand(&manager, lock, P2_PLACE);
        }
        if (FsGetKeyState(FSKEY_ESC)) {
            terminate = 1;
            PushCommand(&manager, lock, USER_TERMINATE);
        }

        if (!manager.IsPlaying()) {
            terminate = 1;
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        manager.Draw();
        FsSwapBuffers();

        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
    // Wait background thread until finishes
    background.join();
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    return 0;
}
