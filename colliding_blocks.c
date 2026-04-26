#include <raylib.h>
#include <stdlib.h>
#include <stdio.h>

#define WIDTH 900
#define HEIGHT 600
#define FLOOR 500
#define WALL 100
#define LINE_THICKNESS 10
#define FRICTION 0
#define LOST_ENERGY 0.0f

typedef struct Position {
    float x;
    float y;
} Position;

typedef struct ExtRectangle {
    Position pos;
    float height; // height = width i guess
    float mass;
    float velocity;
} ExtRectangle;

ExtRectangle smaller_rectangle = (ExtRectangle){(Position){WALL + 60, FLOOR - 60 - (float)LINE_THICKNESS / 2}, 60, 1, -0.0f};
ExtRectangle bigger_rectangle = (ExtRectangle){(Position){WALL + 400, FLOOR - 150 - (float)LINE_THICKNESS / 2}, 150, 1000000, -20.0f};
Vector2 line_pos = (Vector2){WALL, FLOOR};
int collision_counter = 0;
char counter[20];

void applyFriction(float dt, float *velocity) {
    if (*velocity > 0) {
        *velocity -= FRICTION * dt;
        if (*velocity < 0) *velocity = 0;
    } else if (*velocity < 0) {
        *velocity += FRICTION * dt;
        if (*velocity > 0) *velocity = 0;
    }
}

void draw_blocks() {
    DrawRectangle(smaller_rectangle.pos.x, smaller_rectangle.pos.y, smaller_rectangle.height, smaller_rectangle.height, RED);
    DrawRectangle(bigger_rectangle.pos.x, bigger_rectangle.pos.y, bigger_rectangle.height, bigger_rectangle.height, RED);
    snprintf(counter, sizeof(counter), "%d", collision_counter);
    DrawText(counter, 0, 0, 30, WHITE);
}

void resolveCollision(ExtRectangle *a, ExtRectangle *b) {
    float v1 = a->velocity;
    float v2 = b->velocity;
    float m1 = a->mass;
    float m2 = b->mass;
    float e = 1.0f - LOST_ENERGY;

    float new_v1 = (m1 - e*m2)/(m1 + m2) * v1 + ((1+e)*m2)/(m1 + m2) * v2;
    float new_v2 = (m2 - e*m1)/(m1 + m2) * v2 + ((1+e)*m1)/(m1 + m2) * v1;

    a->velocity = new_v1;
    b->velocity = new_v2;
}

void checkCollision() {
    if (smaller_rectangle.pos.x <= WALL + (float)LINE_THICKNESS / 2) {
        smaller_rectangle.pos.x = WALL + (float)LINE_THICKNESS / 2;

        if (smaller_rectangle.velocity < 0) {
            smaller_rectangle.velocity = -smaller_rectangle.velocity;
            collision_counter++; 
        }
    }

    float rightA = smaller_rectangle.pos.x + smaller_rectangle.height;
    float leftB  = bigger_rectangle.pos.x;

    if (rightA > leftB) {
        if (smaller_rectangle.velocity > bigger_rectangle.velocity) {

            float overlap = rightA - leftB;
            float total_mass = smaller_rectangle.mass + bigger_rectangle.mass;

            smaller_rectangle.pos.x -= overlap * (bigger_rectangle.mass / total_mass);
            bigger_rectangle.pos.x += overlap * (smaller_rectangle.mass / total_mass);

            resolveCollision(&smaller_rectangle, &bigger_rectangle);
            collision_counter++;
        }
    }
}

void step(float dt) {
    int substeps = 10000; 
    float sub_dt = dt / substeps;

    for (int i = 0; i < substeps; i++) {
        smaller_rectangle.pos.x += smaller_rectangle.velocity * sub_dt;
        bigger_rectangle.pos.x += bigger_rectangle.velocity * sub_dt;

        checkCollision();
    }

    applyFriction(dt, &smaller_rectangle.velocity);
    applyFriction(dt, &bigger_rectangle.velocity);
}
int main() {
    InitWindow(WIDTH, HEIGHT, "Computing PI with colliding blocks");

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        step(GetFrameTime());
        BeginDrawing();
        ClearBackground(BLACK);
        DrawLineEx(line_pos, (Vector2){WALL, 0}, LINE_THICKNESS, WHITE);
        DrawLineEx(line_pos, (Vector2){WIDTH, FLOOR}, LINE_THICKNESS, WHITE);
        draw_blocks();
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
