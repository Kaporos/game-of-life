#include "./include/raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define CELL_SIZE 10

void step(char* grid, int GRID_WIDTH, int GRID_HEIGHT)
{
    char* copy = malloc(GRID_HEIGHT * GRID_WIDTH);
    memset(copy, 0, GRID_HEIGHT * GRID_WIDTH);
    for (int i = 0; i < GRID_HEIGHT * GRID_WIDTH; i++) {
        copy[i] = grid[i];
    }
    for (int j = 0; j < GRID_WIDTH; j++) {
        for (int i = 0; i < GRID_HEIGHT; i++) {
            int coord = i * GRID_WIDTH + j;
            int sum = 0;
            // for each case, look into neightbors.
            for (int k = -1; k < 2; k++) {
                for (int l = -1; l < 2; l++) {
                    int ii = i + k;
                    int jj = j + l;
                    if (k == 0 && l == 0) {
                        // the actual case does not count
                        continue;
                    }
                    if ((ii * GRID_WIDTH + jj) >= GRID_WIDTH * GRID_HEIGHT || ii < 0 || jj < 0) {
                        // skipping out of bonds
                        continue;
                    }
                    int value = copy[ii * GRID_WIDTH + jj];
                    sum += value;
                }
            }

            // GOL rule
            if (sum < 2 || sum > 3) {
                grid[coord] = 0;
            }
            if (sum == 3) {
                grid[coord] = 1;
            }
        }
    }
    free(copy);
}
int diff(clock_t a, clock_t b)
{
    int v = ((b - a) * 10000) / CLOCKS_PER_SEC;
    return v;
}

int main()
{
    int GRID_WIDTH = 800 / CELL_SIZE;
    int GRID_HEIGHT = 600 / CELL_SIZE;
    char* grid = malloc(GRID_WIDTH * GRID_HEIGHT);
    memset(grid, 0, GRID_WIDTH * GRID_HEIGHT);
    // grid[5] = 1;
    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT | FLAG_WINDOW_TOPMOST);
    SetTargetFPS(60);
    InitWindow(800, 600, "Great window");
    clock_t t = clock();
    int live = 0;
    while (!WindowShouldClose()) {
        BeginDrawing();
        // Drawing current state
        for (int j = 0; j < GRID_WIDTH; j++) {
            for (int i = 0; i < GRID_HEIGHT; i++) {
                char value = grid[i * GRID_WIDTH + j];
                DrawRectangle(j * CELL_SIZE, i * CELL_SIZE, CELL_SIZE, CELL_SIZE, BLACK);
                if (value) {
                    continue;
                }
                DrawRectangle(j * CELL_SIZE, i * CELL_SIZE, CELL_SIZE - 1, CELL_SIZE - 1, WHITE);
            }
        }
        if (IsKeyPressed(KEY_SPACE)) {
            live = !live;
        }
        if (IsKeyPressed(KEY_N)) {
            step(grid, GRID_WIDTH, GRID_HEIGHT);
        }
        if (IsKeyPressed(KEY_R)) {
            // resetting grid to zeros
            memset(grid, 0, GRID_HEIGHT * GRID_WIDTH);
        }
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
            Vector2 position = GetMousePosition();
            int j = (int)position.x / CELL_SIZE;
            int i = (int)position.y / CELL_SIZE;
            grid[i * GRID_WIDTH + j] = 1;
        }
        if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON)) {
            Vector2 position = GetMousePosition();
            int j = (int)position.x / CELL_SIZE;
            int i = (int)position.y / CELL_SIZE;
            grid[i * GRID_WIDTH + j] = 0;
        }
        if (diff(t, clock()) > 30 && live) {
            t = clock();
            step(grid, GRID_WIDTH, GRID_HEIGHT);
        }
        EndDrawing();
    }
    free(grid);
    return 0;
}
