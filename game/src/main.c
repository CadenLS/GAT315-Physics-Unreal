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
#include "integrator.h"

#include <stdlib.h>
#include <assert.h>
#include "World.h"


int main(void)
{
	InitWindow(1280, 720, "Physics Engine"); // initialize window
	SetTargetFPS(60); // set target frames per second (FPS)

	// initialize world
	ncGravity = (Vector2){ 0, 30 };

	// 'game loop'
	while (!WindowShouldClose()) // continue loop until window is closed
	{
		// update
		float dt = GetFrameTime(); // get frame time
		float fps = (float)GetFPS(); // get frames per second

		Vector2 position = GetMousePosition(); // get mouse position
		if (IsMouseButtonDown(0)) // if left mouse button is down
		{
			ncBody* body = CreateBody();
			body->position = GetMousePosition();
			body->mass = GetRandomFloatValue(1, 10);
			body->inverseMass = 1 / body->mass;
			body->type = BT_DYNAMIC;
			body->damping = 2.5f; // how slow it will stop/fall, higher = slower
			body->gravityScale = 20; // faster it falls
			body->color = ColorFromHSV(GetRandomFloatValue(0, 255), GetRandomFloatValue(0, 255), GetRandomFloatValue(0, 255));
			ApplyForce(body, (Vector2){ GetRandomFloatValue(-200, 200), GetRandomFloatValue(-200, 200) }, FM_VELOCITY);
			//body->velocity = CreateVector2(GetRandomFloatValue(-5, 5), GetRandomFloatValue(-5, 5));
			//ApplyForce(body, CreateVector2(GetRandomFloatValue(-50, 50), GetRandomFloatValue(-50, 50)));
		}

		// apply force
		ncBody* body = ncBbodies;
		while (body) // do while we have a valid pointer, will be NULL at the end of the list
		{
			//ApplyForce(body, CreateVector2(0, -100), FM_FORCE);
			body = body->next; // get next body
		}

		// update bodies
		body = ncBbodies;
		while (body) // do while we have a valid pointer, will be NULL at the end of the list
		{
			// update body position
			Step(body, dt);
			body = body->next; // get next body
		}


		// render / draw
		BeginDrawing(); // begin drawing
		ClearBackground(BLACK); // clear the background

		// draw stats
		DrawText(TextFormat("FPS: %.2f (%.2f ms)", fps, 1000 / fps), 10, 10, 20, LIME); // draw FPS
		DrawText(TextFormat("FRAME: %.4f", dt), 10, 30, 20, LIME); // draw frame time

		DrawCircle((int)position.x, (int)position.y, 20, YELLOW); // draw mouse cursor

		// draw bodies
		body = ncBbodies;
		while (body) // do while we have a valid pointer, will be NULL at the end of the list
		{
			// draw body
			DrawCircle((int)body->position.x, body->position.y, body->mass, body->color);

			body = body->next; // get next body
		}

		EndDrawing(); // end drawing
	}

	CloseWindow(); // close the window

	return 0; // return from main function
}