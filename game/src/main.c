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
#include "World.h"

#define MAX_BODIES 100 // define maximum number of bodies


int main(void)
{
	InitWindow(1280, 720, "Physics Engine"); // initialize window
	SetTargetFPS(60); // set target frames per second (FPS)

	// 'game loop'
	while (!WindowShouldClose()) // continue loop until window is closed
	{
		// update
		float dt = GetFrameTime(); // get frame time
		float fps = (float)GetFPS(); // get frames per second

		Vector2 position = GetMousePosition(); // get mouse position
		if (IsMouseButtonDown(0)) // if left mouse button is down
		{
			Body* body = CreateBody();
			body->position = GetMousePosition();
			body->velocity = CreateVector2(GetRandomFloatValue(-5, 5), GetRandomFloatValue(-5, 5));
		}

		// render / draw
		BeginDrawing(); // begin drawing
		ClearBackground(BLACK); // clear the background

		// draw stats
		DrawText(TextFormat("FPS: %.2f (%.2f ms)", fps, 1000 / fps), 10, 10, 20, LIME); // draw FPS
		DrawText(TextFormat("FRAME: %.4f", dt), 10, 30, 20, LIME); // draw frame time

		DrawCircle((int)position.x, (int)position.y, 20, RED); // draw mouse cursor

		// update / draw bodies
		Body* body = bodies;
		while (body) // do while we have a valid pointer, will be NULL at the end of the list
		{
			// update body position
			body->position = Vector2Add(body->position, body->velocity);
			// draw body
			DrawCircle((int)body->position.x, body->position.y, 10, RED);

			body = body->next; // get next body
		}

		EndDrawing(); // end drawing
	}

	CloseWindow(); // close the window

	return 0; // return from main function
}