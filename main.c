#include <stdio.h>
#include <stdbool.h>
#include <raylib.h>
#include <string.h>
#include <stdlib.h>

#define SCREENSIZE 800
#define CELLSIZE 10
#define CELLSPERROW (SCREENSIZE/CELLSIZE)

void updateGame(bool *cells, bool *successor) {
	int neighborIndex;
	int liveNeighbors;
	for(int i = 0; i < CELLSPERROW*CELLSPERROW; i++) {
		liveNeighbors = 0;
		for (int n = 0; n < 9; n++) {
			if (n == 4) continue;

			neighborIndex = i;
			neighborIndex += ((n/3)-1) * CELLSPERROW; // Vertical offset
			neighborIndex += ((n%3)-1); // Horizontal offset
			if (neighborIndex >= 0 && neighborIndex < CELLSPERROW*CELLSPERROW) {
				if (cells[neighborIndex]) liveNeighbors++;
			}
		}

		successor[i] = false;
		if (!cells[i] && liveNeighbors == 3) {
			successor[i] = true;
		} else if (cells[i] && liveNeighbors >= 2 && liveNeighbors <= 3) {
			successor[i] = true;
		}
	}
}

int main() {
	bool *cells = calloc(CELLSPERROW*CELLSPERROW, sizeof(bool));
	bool *successor = calloc(CELLSPERROW*CELLSPERROW, sizeof(bool));
	cells[1] = true;
	cells[82] = true;
	cells[160] = true;
	cells[161] = true;
	cells[162] = true;

	InitWindow(SCREENSIZE, SCREENSIZE, "Game of Life");
	SetTargetFPS(15);
	while (!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(RAYWHITE);
		for(int i = 0; i < CELLSPERROW*CELLSPERROW; i++) {
			int row = i % CELLSPERROW;
			int col = i / CELLSPERROW;
			if (cells[i]) {
				DrawRectangle(row*CELLSIZE, col*CELLSIZE, CELLSIZE, CELLSIZE, BLACK);
			}
		}
		EndDrawing();

		updateGame(cells, successor);
		bool *tmp = cells;
		cells = successor;
		successor = tmp;
	}
	free(cells);
	free(successor);
	return 0;
}
