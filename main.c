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

int moveCursor(int cursor, int n) {
	cursor += n;
	if (cursor < 0) {
		return 0;
	}
	if (cursor >= TOTALCELLS) {
		return TOTALCELLS-1;
	}
	return cursor;
}

int main() {
	bool *cells = calloc(TOTALCELLS, sizeof(bool));
	bool *successor = calloc(TOTALCELLS, sizeof(bool));
	bool *initial = calloc(TOTALCELLS, sizeof(bool));

	// Amongus pattern produces pretty complex results.
	int center = (TOTALCELLS/2) + (CELLSPERROW/2);
	cells[center] = true;
	cells[center+1] = true;
	cells[center+2] = true;
	cells[center+CELLSPERROW-1] = true;
	cells[center+CELLSPERROW] = true;
	cells[center+2*CELLSPERROW-1] = true;
	cells[center+2*CELLSPERROW] = true;
	cells[center+2*CELLSPERROW+1] = true;
	cells[center+2*CELLSPERROW+2] = true;
	cells[center+3*CELLSPERROW] = true;
	cells[center+3*CELLSPERROW+1] = true;
	cells[center+3*CELLSPERROW+2] = true;
	cells[center+4*CELLSPERROW] = true;
	cells[center+4*CELLSPERROW+2] = true;

	memcpy(initial, cells, sizeof(bool) * TOTALCELLS);

	float simulationRate = 50.0f;  // updates/s
	float deltaTime = 0.0f;
	float sinceLastUpdate = 0.0f;
	bool paused = true;
	int cursor = 0;
	bool cursorEnabled = true;

	InitWindow(SCREENSIZE, SCREENSIZE+50, "Game of Life");
	SetTargetFPS(60);
	while (!WindowShouldClose()) {
		// Check keyboard input
		int key;
		while ((key = GetKeyPressed())) {
			switch (key) {
				case KEY_P:
					paused = !paused;
					break;
				case KEY_R:
					memcpy(cells, initial, sizeof(bool) * TOTALCELLS);
					break;
				case KEY_UP:
					cursor = moveCursor(cursor, -CELLSPERROW);
					break;
				case KEY_DOWN:
					cursor = moveCursor(cursor, CELLSPERROW);
					break;
				case KEY_LEFT:
					cursor = moveCursor(cursor, -1);
					break;
				case KEY_RIGHT:
					cursor = moveCursor(cursor, 1);
					break;
				case KEY_SPACE:
					cells[cursor] = true;
					initial[cursor] = true;
					break;
				case KEY_X:
					memset(cells, 0, sizeof(bool)*TOTALCELLS);
					break;
			}
		}

		// Draw
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
		int cursorRow = cursor % CELLSPERROW;
		int cursorCol = cursor / CELLSPERROW;
		if (cursorEnabled) {
			DrawRectangle(cursorRow * CELLSIZE, cursorCol * CELLSIZE,
				      CELLSIZE, CELLSIZE, RED);
		}
		DrawText("P - Pause, X - Clear, R - Reset\nUp/Down/Left/Right - Move Cursor, Space - Set Cell to Alive", 20, SCREENSIZE, 20, BLACK);
		EndDrawing();

		// Update game state
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
