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
						    
			// Boundary conditions
			if (
					neighborIndex < 0
					|| neighborIndex >= CELLSPERROW*CELLSPERROW 
					|| (i % CELLSPERROW == 0 && ((n%3)-1) < 0)
					|| (i % CELLSPERROW == CELLSPERROW-1 && ((n%3)-1) > 0)
			   ) {
				continue;
			}

			if (cells[neighborIndex]) liveNeighbors++;
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
	cells[81] = true;
	cells[162] = true;
	cells[240] = true;
	cells[241] = true;
	cells[242] = true;

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
