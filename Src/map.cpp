#include "map.h"
using namespace std;
using namespace chrono;

Bitmap::Bitmap() {
    pix = nullptr;
    wid = COLUMN;
    hei = ROW;

    Create(wid, hei);
    if (YSOK == png[0].Decode("solid.png")) {
        png[0].Flip();
        printf("%d x %d\n", png[0].wid, png[0].hei);
    } else {
        printf("Could not read the image.\n");
    }

    if (YSOK == png[1].Decode("breakable.png")) {
        png[1].Flip();
        printf("%d x %d\n", png[1].wid, png[1].hei);
    } else {
        printf("Could not read the image.\n");
    }

    if (YSOK == png[2].Decode("morebubble.png")) {
        png[2].Flip();
        printf("%d x %d\n", png[2].wid, png[2].hei);
    } else {
        printf("Could not read the image.\n");
    }

    if (YSOK == png[3].Decode("MorePowerful.png")) {
        png[3].Flip();
        printf("%d x %d\n", png[3].wid, png[3].hei);
    } else {
        printf("Could not read the image.\n");
    }
    if (YSOK == png[4].Decode("SpeedUp.png")) {
        png[2].Flip();
        printf("%d x %d\n", png[4].wid, png[4].hei);
    } else {
        printf("Could not read the image.\n");
    }
    if (YSOK == pngbackgroud.Decode("background.png")) {
        pngbackgroud.Flip();
        printf("%d x %d\n", pngbackgroud.wid, pngbackgroud.hei);
    } else {
        printf("Could not read the image.\n");
    }
}

Bitmap::~Bitmap() {
    CleanUp();
}

void Bitmap::CleanUp(void) {
    wid = 0;
    hei = 0;
    if (nullptr != pix) {
        delete[] pix;
    }
    pix = nullptr;
}

void Bitmap::Create(int w, int h) {
    CleanUp();

    pix = new GridStatus[w * h];
    wid = w;
    hei = h;
    GridStatus mapinitial[COLUMN * ROW] = {
        GridFree,           GridFree,           GridFree,
        GridFree,           GridIndestructible, GridFree,
        GridFree,           GridFree,           GridFree,
        GridFree,           GridDestructible,   GridFree,
        GridFree,           GridFree,           GridFree,
        GridFree,           GridFree,           GridFree,
        GridFree,           GridIndestructible, GridFree,
        GridFree,           GridFree,           GridFree,
        GridFree,           GridIndestructible, GridFree,
        GridFree,           GridFree,           GridFree,
        GridFree,           GridFree,           GridFree,
        GridFree,           GridDestructible,   GridFree,
        GridFree,           GridFree,           GridFree,
        GridFree,           GridIndestructible, GridFree,
        GridFree,           GridFree,           GridFree,
        GridDestructible,   GridIndestructible, GridIndestructible,
        GridDestructible,   GridDestructible,   GridDestructible,
        GridDestructible,   GridIndestructible, GridDestructible,
        GridDestructible,   GridDestructible,   GridDestructible,
        GridDestructible,   GridIndestructible, GridIndestructible,
        GridFree,           GridFree,           GridFree,
        GridDestructible,   GridFree,           GridFree,
        GridDestructible,   GridIndestructible, GridDestructible,
        GridFree,           GridFree,           GridDestructible,
        GridFree,           GridFree,           GridFree,
        GridFree,           GridFree,           GridFree,
        GridDestructible,   GridFree,           GridFree,
        GridDestructible,   GridDestructible,   GridDestructible,
        GridFree,           GridFree,           GridDestructible,
        GridFree,           GridFree,           GridFree,
        GridFree,           GridFree,           GridFree,
        GridIndestructible, GridDestructible,   GridDestructible,
        GridDestructible,   GridIndestructible, GridDestructible,
        GridDestructible,   GridDestructible,   GridIndestructible,
        GridFree,           GridFree,           GridFree,
        GridFree,           GridFree,           GridFree,
        GridDestructible,   GridFree,           GridFree,
        GridDestructible,   GridDestructible,   GridDestructible,
        GridFree,           GridFree,           GridDestructible,
        GridFree,           GridFree,           GridFree,
        GridFree,           GridFree,           GridFree,
        GridDestructible,   GridFree,           GridFree,
        GridDestructible,   GridDestructible,   GridDestructible,
        GridFree,           GridFree,           GridDestructible,
        GridFree,           GridFree,           GridFree,
        GridIndestructible, GridIndestructible, GridDestructible,
        GridDestructible,   GridDestructible,   GridDestructible,
        GridDestructible,   GridIndestructible, GridDestructible,
        GridDestructible,   GridDestructible,   GridDestructible,
        GridIndestructible, GridIndestructible, GridDestructible,
        GridFree,           GridFree,           GridFree,
        GridFree,           GridIndestructible, GridFree,
        GridFree,           GridFree,           GridFree,
        GridFree,           GridIndestructible, GridFree,
        GridFree,           GridFree,           GridFree,
        GridFree,           GridFree,           GridFree,
        GridFree,           GridIndestructible, GridFree,
        GridFree,           GridFree,           GridFree,
        GridFree,           GridIndestructible, GridFree,
        GridFree,           GridFree,           GridFree,
        GridFree,           GridFree,           GridFree,
        GridFree,           GridDestructible,   GridFree,
        GridFree,           GridFree,           GridFree,
        GridFree,           GridDestructible,   GridFree,
        GridFree,           GridFree,           GridFree};

    for (int i = 0; i < wid * hei; ++i) {
        pix[i] = mapinitial[i];
    }
}

void Bitmap::SetPixel(int x, int y, GridStatus p) {
    if (0 <= x && x < wid && 0 <= y && y < hei) {
        pix[y * wid + x] = p;
        cout << p << "setpix"
             << "\n";
    }
    for (int i = 0; i < wid * hei; i++) {
        cout << int(pix[i]) << ",";
    }
    cout << "\n";
}

GridStatus Bitmap::GetPixel(int x, int y) const {
    if (0 <= x && x < wid && 0 <= y && y < hei) {
        return pix[y * wid + x];
    }
    return GridInvalid;
}

void Bitmap::Drawbackgroud() const {
    glRasterPos2d(0, 779);
    glDrawPixels(pngbackgroud.wid, pngbackgroud.hei, GL_RGBA, GL_UNSIGNED_BYTE,
                 pngbackgroud.rgba);
}

void Bitmap::Draw() const {
    Drawbackgroud();
    glRasterPos2d(100, 100);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    for (int y = 0; y < hei; ++y) {
        for (int x = 0; x < wid; ++x) {
            auto pix = GetPixel(x, y);

            switch (pix) {
            case GridIndestructible: {
                glRasterPos2d(x * RESOLUTION, (y + 1) * RESOLUTION - 1);
                glDrawPixels(png[0].wid, png[0].hei, GL_RGBA, GL_UNSIGNED_BYTE,
                             png[0].rgba);
                break;
            }
            case GridDestructible: {
                glRasterPos2d(x * RESOLUTION, (y + 1) * RESOLUTION - 1);
                glDrawPixels(png[1].wid, png[1].hei, GL_RGBA, GL_UNSIGNED_BYTE,
                             png[1].rgba);
                break;
            }
            case GridMoreBubble: {
                glRasterPos2d(x * RESOLUTION, (y + 1) * RESOLUTION - 1);
                glDrawPixels(png[2].wid, png[2].hei, GL_RGBA, GL_UNSIGNED_BYTE,
                             png[2].rgba);
                break;
            }
            case GridLongerBubble: {
                glRasterPos2d(x * RESOLUTION, (y + 1) * RESOLUTION - 1);
                glDrawPixels(png[3].wid, png[3].hei, GL_RGBA, GL_UNSIGNED_BYTE,
                             png[3].rgba);
                break;
            }
            case GridSpeedUp: {
                glRasterPos2d(x * RESOLUTION, (y + 1) * RESOLUTION - 1);
                glDrawPixels(png[3].wid, png[4].hei, GL_RGBA, GL_UNSIGNED_BYTE,
                             png[4].rgba);
                break;
            }
            default:
                break;
            }
        }
    }
}
bool Bitmap::Reachable(int x, int y) const {
    // convert pixel coordinate to grid

    return true;
    if (x <= 0 || x >= wid*RESOLUTION || y <= 0 || y >= hei*RESOLUTION){
        return false;
    }
    x = x / RESOLUTION;
    y = y / RESOLUTION;
    
    GridStatus status = GetPixel(x, y);
    if (status == GridDestructible || status == GridIndestructible) {
        return false;
    }
    return true;
}

/*
int main(void) {
    int wid = COLUMN, hei = ROW;

    Bitmap bmp;

    FsOpenWindow(0, 0, wid * RESOLUTION, hei * RESOLUTION, 1);

    for (;;) {
        FsPollDevice();
        auto key = FsInkey();
        int lb, mb, rb, mx, my;
        auto evt = FsGetMouseEvent(lb, mb, rb, mx, my);

        if (FSKEY_ESC == key) {
            break;
        }
        if (FSKEY_0 <= key && key <= FSKEY_7) {
            GridStatus colorCode;
            int px = mx / RESOLUTION;
            int py = my / RESOLUTION;
            switch (key) {
            case FSKEY_0:
                colorCode = GridFree;
                break;
            case FSKEY_1:
                colorCode = GridIndestructible;
                break;
            case FSKEY_2:
                colorCode = GridDestructible;
                break;
            case FSKEY_3:
                colorCode = GridSpeedUp;
                break;
            case FSKEY_4:
                colorCode = GridMoreBubble;
                break;
            case FSKEY_5:
                colorCode = GridLongerBubble;
                break;
            default:
                break;
            }

            cout << colorCode << ',';
            bmp.SetPixel(px, py, colorCode);
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        bmp.Draw();
        FsSwapBuffers();

        FsSleep(25);
    }
}
*/
