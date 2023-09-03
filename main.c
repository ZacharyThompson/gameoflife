#include <stdio.h>
#include <raylib.h>

int main() {
	InitWindow(800, 400, "Hello World");
	SetTargetFPS(60);
	while (!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(BLUE);
		DrawText("Hello World!!!", 0, 0, 20, RAYWHITE);
		EndDrawing();
	}
	return 0;
}
