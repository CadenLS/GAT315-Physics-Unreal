/*******************************************************************************************
*
*   raylib game template
*
*   <Game title>
*   <Game description>
*
*   This game has been created using raylib (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2021 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "Body.h"
#include "Mathf.h"
#include "raylib.h"
#include "raymath.h"

#include <stdlib.h>
#include <assert.h>

#define MAX_BODIES 100 // define maximum number of bodies

int main(void)
{
	InitWindow(1280, 720, "Physics Engine"); // initialize window
	SetTargetFPS(60); // set target frames per second (FPS)

	Body* bodies = (Body*)malloc(sizeof(Body) * MAX_BODIES); // allocate memory for bodies array
	assert(bodies); // assert that memory allocation was successful

	int bodyCount = 0; // initialize body count

	// 'game loop'
	while (!WindowShouldClose()) // continue loop until window is closed
	{
		// update
		float dt = GetFrameTime(); // get frame time
		float fps = (float)GetFPS(); // get frames per second

		Vector2 position = GetMousePosition(); // get mouse position
		if (IsMouseButtonDown(0)) // if left mouse button is down
		{
			bodies[bodyCount].position = position; // set body position to mouse position
			bodies[bodyCount].velocity = CreateVector2(GetRandomFloatValue(-5, 5), GetRandomFloatValue(-5, 5)); // set random velocity
			bodyCount++; // increment body count
		}

		// render / draw
		BeginDrawing(); // begin drawing
		ClearBackground(BLACK); // clear the background

		// draw stats
		DrawText(TextFormat("FPS: %.2f (%.2f ms)", fps, 1000 / fps), 10, 10, 20, LIME); // draw FPS
		DrawText(TextFormat("FRAME: %.4f", dt), 10, 30, 20, LIME); // draw frame time

		DrawCircle((int)position.x, (int)position.y, 20, RED); // draw mouse cursor

		// update bodies
		for (int i = 0; i < bodyCount; i++) // loop through each body
		{
			bodies[i].position = Vector2Add(bodies[i].position, bodies[i].velocity); // update body position

			DrawCircle((int)bodies[i].position.x, (int)bodies[i].position.y, 20, YELLOW); // draw body
		}

		EndDrawing(); // end drawing
	}

	CloseWindow(); // close the window
	free(bodies); // free memory allocated for bodies

	return 0; // return from main function
}