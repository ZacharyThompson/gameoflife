#include <raylib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SCREENSIZE 800
#define CELLSIZE 10
#define CELLSPERROW (SCREENSIZE / CELLSIZE)
#define TOTALCELLS CELLSPERROW *CELLSPERROW

bool neighborInBounds(int i, int n) {
	int nIdx = i;			      // Neighbor Index
	nIdx += ((n / 3) - 1) * CELLSPERROW;  // Vertical offset
	nIdx += ((n % 3) - 1);		      // Horizontal offset
	return nIdx >= 0 && nIdx < TOTALCELLS &&
	       !(i % CELLSPERROW == 0 && ((n % 3) - 1) < 0) &&
	       !(i % CELLSPERROW == CELLSPERROW - 1 && ((n % 3) - 1) > 0);
}

void updateGame(bool *cells, bool *successor) {
	int neighborIndex;
	int liveNeighbors;
	for (int i = 0; i < TOTALCELLS; i++) {
		liveNeighbors = 0;
		for (int n = 0; n < 9; n++) {
			if (n == 4) continue;

			neighborIndex = i;
			neighborIndex +=
				((n / 3) - 1) * CELLSPERROW;  // Vertical offset
			neighborIndex += ((n % 3) - 1);	 // Horizontal offset

			// Boundary conditions
			if (!neighborInBounds(i, n)) continue;

			if (cells[neighborIndex]) liveNeighbors++;
		}

		successor[i] = false;
		if (!cells[i] && liveNeighbors == 3) {
			successor[i] = true;
		} else if (cells[i] && liveNeighbors >= 2 &&
			   liveNeighbors <= 3) {
			successor[i] = true;
		}
	}
}

int main() {
	bool *cells = calloc(TOTALCELLS, sizeof(bool));
	bool *successor = calloc(TOTALCELLS, sizeof(bool));
	bool *initial = calloc(TOTALCELLS, sizeof(bool));

	/*
	   cells[1] = true;
	   cells[82] = true;
	   cells[160] = true;
	   cells[161] = true;
	   cells[162] = true;
	   */

	size_t voffset = 30 * 80;
	size_t hoffset = 50;
	cells[1 + voffset + hoffset] = true;
	cells[2 + voffset + hoffset] = true;
	cells[80 + voffset + hoffset] = true;
	cells[81 + voffset + hoffset] = true;
	cells[161 + voffset + hoffset] = true;

	memcpy(initial, cells, sizeof(bool) * TOTALCELLS);

	float simulationRate = 50.0f;  // updates/s
	float deltaTime = 0.0f;
	float sinceLastUpdate = 0.0f;
	bool paused = false;
	InitWindow(SCREENSIZE, SCREENSIZE, "Game of Life");
	SetTargetFPS(60);
	while (!WindowShouldClose()) {
		if (paused) {
			paused = !IsKeyPressed(KEY_P);
		} else {
			paused = IsKeyPressed(KEY_P);
		}
		if (IsKeyPressed(KEY_R)) {
			memcpy(cells, initial, sizeof(bool) * TOTALCELLS);
		}
		BeginDrawing();
		ClearBackground(RAYWHITE);
		for (int i = 0; i < TOTALCELLS; i++) {
			int row = i % CELLSPERROW;
			int col = i / CELLSPERROW;
			if (cells[i]) {
				DrawRectangle(row * CELLSIZE, col * CELLSIZE,
					      CELLSIZE, CELLSIZE, BLACK);
			}
		}
		EndDrawing();

		deltaTime = GetFrameTime();
		sinceLastUpdate += deltaTime;
		if (sinceLastUpdate >= 1 / simulationRate && !paused) {
			updateGame(cells, successor);
			sinceLastUpdate -= 1 / simulationRate;
			bool *tmp = cells;
			cells = successor;
			successor = tmp;
		}
	}

	free(cells);
	free(successor);
	free(initial);
	return 0;
}
