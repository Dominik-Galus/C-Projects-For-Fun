#include <stdio.h>
#include <raylib.h>
#include <math.h>

#define WIDTH 900
#define HEIGHT 600
#define SEGMENTS_LEN_MAX 9
#define LENGTH_DIV_FACTOR 1.4
#define BRANCH_LENGTH 125
#define THICK 12
#define SPREAD_ANGLE_1 25
#define SPREAD_ANGLE_2 45

Vector2 start_pos = (Vector2){(float)WIDTH / 2, (float)HEIGHT};

void draw_branch(Vector2 pos, float phi, float len, float thick, int current_segment) {
    if (current_segment >= SEGMENTS_LEN_MAX) {
        return;
    }
    Vector2 end_pos = (Vector2){pos.x + len * cosf(phi * DEG2RAD), pos.y - len * sinf(phi * DEG2RAD)};
    DrawLineEx(pos, end_pos, thick, WHITE);
    draw_branch(end_pos, phi + SPREAD_ANGLE_1, len / LENGTH_DIV_FACTOR, thick / LENGTH_DIV_FACTOR, current_segment + 1);
    draw_branch(end_pos, phi - SPREAD_ANGLE_2, len / LENGTH_DIV_FACTOR, thick / LENGTH_DIV_FACTOR, current_segment + 1);
}

void draw_tree() {
    Vector2 end_pos = (Vector2){start_pos.x, start_pos.y - BRANCH_LENGTH};
    DrawLineEx(start_pos, end_pos, THICK, WHITE);
    draw_branch(end_pos, 90 + SPREAD_ANGLE_1, BRANCH_LENGTH / LENGTH_DIV_FACTOR, THICK / LENGTH_DIV_FACTOR, 0);
    draw_branch(end_pos, 90 - SPREAD_ANGLE_2, BRANCH_LENGTH / LENGTH_DIV_FACTOR, THICK / LENGTH_DIV_FACTOR, 0);
}


int main(int argc, char *argv[]) {
    InitWindow(WIDTH, HEIGHT, "Fractal tree");

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        draw_tree();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
