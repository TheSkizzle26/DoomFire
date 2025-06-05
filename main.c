#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <raylib.h>

#define SW 1920
#define SH 1080
#define RES 4
#define NUM_REPS 2 // number of repetitions, useful for high resolutions
#define TARGET_FPS 0 // 0 for unlimited

constexpr int RW = (SW / RES / NUM_REPS);
constexpr int RH = (SH / RES);

#define SHOW_FPS true
#define FULLSCREEN true


Color palette[37];

unsigned short int firePixels[RW * RH]; // simulation data, 0-36 per pixel
Texture2D fireTexture;
Color fireTexturePixels[RW * RH]; // stores pixels of fireTexture, used for updating fireTexture

int highestRenderedPixel = RH; // only update/render pixels that are getting updated


// helper functions
int max(const int a, const int b) {
    if (a > b) {
        return a;
    }
    return b;
}

double dmod(double x, double y) { // modulo using doubles
    return x - (int)(x/y) * y;
}


void initPalette() {
    const int numColors = 4;
    Color colors[] = {
        {0, 0, 0, 255},
        {255, 70, 0, 255},
        {253, 217, 88, 255},
        {255, 255, 255, 255}
    };

    for (int i = 0; i < 37; i++) {
        const double t = (double)i / 37.0;

        const double delta = 1.0 / (double)(numColors - 1);
        const int startIndex = (int)(t / delta);

        const double localT = dmod(t, delta) / delta;

        palette[i] = ColorLerp(colors[startIndex], colors[startIndex + 1], (float)localT);
    }
}

void init() {
    fireTexture = LoadTextureFromImage(GenImageColor(RW, RH, BLACK));

    // set generator (bottom row) of firePixels
    for (int x = 0; x < RW; x++) {
        firePixels[(RH-1) * RW + x] = 36;
    }

    initPalette();
}

void spreadFire(const int from) {
    if (firePixels[from] == 0) {
        firePixels[from - RW] = 0;
        return;
    }

    const int rng = (int)(GetRandomValue(0, RAND_MAX) * 3 + 0.5) & 3;
    const int to = from - RW - rng + 1;
    firePixels[to] = max(firePixels[from] - (rng & 1), 0);
}

void update() {
    for (int x = 0; x < RW; x++) {
        for (int y = max(highestRenderedPixel-1, 1); y < RH; y++) {
            spreadFire(y * RW + x);
        }
    }
}

void setPixel(const int x, const int y, const Color color) {
    fireTexturePixels[y * RW + x] = color;

    if (y < highestRenderedPixel) { // "<", because 0, 0 is top left
        if (color.r != 0 || color.g != 0 || color.b != 0) {
            highestRenderedPixel = y;
        }
    }
}

void renderFire() { // render fire to fireTexture
    for (int x = 0; x < RW; x++) {
        for (int y = max(highestRenderedPixel-1, 0); y < RH; y++) {
            setPixel(x, y, palette[firePixels[y * RW + x]]);
        }
    }
}

void render() {
    if (SHOW_FPS) { // only clear when showing fps, not needed otherwise
        ClearBackground(BLACK);
    }

    renderFire();
    UpdateTexture(fireTexture, fireTexturePixels);
    for (int i = 0; i < NUM_REPS; i++) { // render repetitions
        DrawTextureEx(fireTexture, (Vector2){(float)(i * RW * RES), 0}, 0, RES, WHITE);
    }

    EndDrawing();
}


int main(void) {
    InitWindow(SW, SH, "Doom Fire");
    SetTargetFPS(TARGET_FPS);
    if (FULLSCREEN) {
        ToggleFullscreen();
    }

    init();

    while (!WindowShouldClose()) {
        update();
        render();

        if (SHOW_FPS) { // not in render() because it crashed there???
            char fpsString[0];
            sprintf(fpsString, "FPS: %d", GetFPS());
            DrawText(fpsString, 0, 0, 50, WHITE);
        }
    }

    // unload textures from vram
    UnloadTexture(fireTexture);

    return 0;
}