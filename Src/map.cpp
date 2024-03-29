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
        // png[2].Flip();
        printf("%d x %d\n", png[2].wid, png[2].hei);
    } else {
        printf("Could not read the image.\n");
    }

    if (YSOK == png[3].Decode("morepowerful.png")) {
        png[3].Flip();
        printf("%d x %d\n", png[3].wid, png[3].hei);
    } else {
        printf("Could not read the image.\n");
    }
    if (YSOK == png[4].Decode("SpeedUp.png")) {
        // png[4].Flip();
        printf("%d x %d\n", png[4].wid, png[4].hei);
    } else {
        printf("Could not read the image.\n");
    }
    if (YSOK == png[5].Decode("boom1.png")) {
        png[5].Flip();
        printf("%d x %d\n", png[5].wid, png[5].hei);
    } else {
        printf("Could not read the image.\n");
    }
    if (YSOK == png[6].Decode("boom2.png")) {
        png[6].Flip();
        printf("%d x %d\n", png[6].wid, png[6].hei);
    } else {
        printf("Could not read the image.\n");
    }
    if (YSOK == png[7].Decode("exploding_mid.png")) {
        png[7].Flip();
        printf("%d x %d\n", png[7].wid, png[7].hei);
    } else {
        printf("Could not read the image.\n");
    }
    if (YSOK == png[8].Decode("exploding_top.png")) {
        png[8].Flip();
        printf("%d x %d\n", png[8].wid, png[8].hei);
    } else {
        printf("Could not read the image.\n");
    }
    if (YSOK == png[9].Decode("exploding_top.png")) {
        // Do NOT flip, representing downwards
        printf("%d x %d\n", png[9].wid, png[9].hei);
    } else {
        printf("Could not read the image.\n");
    }
    if (YSOK == png[10].Decode("exploding_left.png")) {
        png[10].Flip();
        printf("%d x %d\n", png[10].wid, png[10].hei);
    } else {
        printf("Could not read the image.\n");
    }
    if (YSOK == png[11].Decode("exploding_left.png")) {
        // Do NOT flip, representing rightwards
        printf("%d x %d\n", png[11].wid, png[11].hei);
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

void Bitmap::SetGrid(int x, int y, GridStatus p) {
    if (0 <= x && x < wid && 0 <= y && y < hei) {
        pix[y * wid + x] = p;
    }
}

GridStatus Bitmap::GetGrid(int x, int y) const {
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
            int pix = (int)GetGrid(x, y) - (int)GridIndestructible;
            if (pix < 0 || pix > 11) {
                if (pix > 11) {
                    std::cout << "[ERROR] Found error status, row: " << y
                              << ", col: " << x << ", value: " << pix
                              << std::endl;
                }
                continue;
            }

            glRasterPos2d(x * RESOLUTION, (y + 1) * RESOLUTION - 1);
            glDrawPixels(png[pix].wid, png[pix].hei, GL_RGBA, GL_UNSIGNED_BYTE,
                         png[pix].rgba);
        }
    }
}
bool Bitmap::Reachable(int x, int y) const {
    // convert pixel coordinate to grid
    if (x <= 0 || x >= wid * RESOLUTION || y <= 0 || y >= hei * RESOLUTION) {
        return false;
    }
    x = x / RESOLUTION;
    y = y / RESOLUTION;

    GridStatus status = GetGrid(x, y);
    if (status == GridDestructible || status == GridIndestructible) {
        return false;
    }
    return true;
}

