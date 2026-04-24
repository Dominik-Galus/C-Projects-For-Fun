#include <stdio.h>
#include <raylib.h>
#include <stdlib.h>
#include <math.h>

#define WIDTH 900
#define HEIGHT 600

#define RADIUS 120
#define SEGMENTS 128
#define HOVERED_SIZE 20

typedef struct Data {
    char *name;
    float amount;
} Data;

typedef struct Angles {
    float start;
    float end;
} Angles;

Vector2 start_pos = (Vector2){(float)WIDTH / 2, (float)HEIGHT / 2};
float total_amount = 0;
Angles *angles;
int hovered = -1;

Color colors[] = {WHITE, RED, BLUE, GREEN};

void checkHovered(int size) {
    Vector2 position = GetMousePosition();
    float distance = sqrt(pow(start_pos.x - position.x, 2) + pow(start_pos.y - position.y, 2));

    hovered = -1; 

    if (distance < RADIUS) {
        float angle = atan2f(position.y - start_pos.y, position.x - start_pos.x) * RAD2DEG;

        if (angle < 0) {
            angle += 360.0f;
        }

        for (int i = 0; i < size; i++) {
            if (angle >= angles[i].start && angle <= angles[i].end) {
                hovered = i;
                break;
            }
        }
    }
}

void draw_piechart(Data data[], int size) {
    checkHovered(size);
    for (int i = 0; i < size; i++) {
        if (hovered == i) {
            DrawCircleSector(start_pos, RADIUS + HOVERED_SIZE, angles[i].start, angles[i].end, SEGMENTS, colors[i % size]);
        }
        else {
            DrawCircleSector(start_pos, RADIUS, angles[i].start, angles[i].end, SEGMENTS, colors[i % size]);
        }
    }
}

void compute_chart(Data data[], int size) {
    total_amount = 0;

    for (int i = 0; i < size; i++) total_amount += data[i].amount;
    angles = malloc(size * sizeof(Angles));
    for (int i = 0; i < size; i++) {
        if (i == 0) {
            angles[i].start = 0;
            angles[i].end = 360 * (data[i].amount / total_amount);
        }
        else {
            angles[i].start = angles[i - 1].end;
            angles[i].end = angles[i - 1].end + (360 * (data[i].amount / total_amount));
        }
    }
}

int main() {
    InitWindow(WIDTH, HEIGHT, "Interactive Pie Chart");

    Data example_data[] = {
        (Data){"money", 2555.123},
        (Data){"media", 6546.43},
        (Data){"job", 1212.5454},
        (Data){"woman", 2652.121},
    };
    int size = (int)sizeof(example_data) / sizeof(Data);
    compute_chart(example_data, size);

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        draw_piechart(example_data, size);
        EndDrawing();
    }

    CloseWindow();

    free(angles);
    return 0;
}

