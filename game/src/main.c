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
#include "force.h"
#include "raylib.h"
#include "raymath.h"
#include "integrator.h"
#include "render.h"
#include "editor.h"
#include "spring.h"
#include "collision.h"
#include "contact.h"

#include <stdlib.h>
#include <assert.h>
#include "World.h"


// Define trail length and fade rate
#define TRAIL_LENGTH 50 
#define TRAIL_FADE_RATE 0.02 
#define MAX_PARTICLES 10
#define MAX_GLOWS 10
#define PARTICLE_LIFETIME 2.0f 

typedef struct {
	Vector2 position;
	Color color;
	float radius;
	float alpha;
} Glow;

// Function to draw a fading line segment
void DrawFadingLine(Vector2 start, Vector2 end, Color color, float fade, float size)
{
	float smoothFade = (1.0f - fade) * fade;
	DrawLineEx(ConvertWorldToScreen(start), ConvertWorldToScreen(end), size, Fade(color, smoothFade * 255));
}


// Function to draw a fading glow circle
void DrawFadingGlow(Vector2 center, float radius, Color color, float alpha)
{
	for (int i = 0; i < 10; i++) // Draw 10 circles with decreasing alpha for the fading effect
	{
		float currentAlpha = alpha * ((float)(10 - i) / 10.0f); // Decrease alpha for each circle
		Color currentColor = Fade(color, currentAlpha * 0.05f); // Fade color based on alpha
		DrawCircleV(center, radius + (i * 2.0f), currentColor); // Increase radius for each circle
	}
}

int main(void)
{
	ncBody* selectedBody = NULL;
	ncBody* connectBody = NULL;
	ncContact_t* contacts = NULL;

	float fixedTimestep = 1.0f / ncEditorData.FixedTimeStep;
	float timeAccumulator = 0; 

	InitWindow(1920, 1080, "Caden Sinclair - Physics Engine"); // initialize window
	InitEditor();
	SetTargetFPS(60); // set target frames per second (FPS)

	// initialize world
	ncGravity = (Vector2){ 0, -1 };

	// Arrays for glows and glow timers
	Glow glows[MAX_GLOWS] = { 0 }; 
	float glowTimers[MAX_GLOWS] = { 0 };

	// 'game loop'
	while (!WindowShouldClose()) // continue loop until window is closed
	{
		// update
		float dt = GetFrameTime(); // get frame time
		float fps = (float)GetFPS(); // get frames per second
		ncGravity = (Vector2){ 0, -ncEditorData.GravityValue };
		bodyC = ncEditorData.BodyColor;

		Vector2 position = GetMousePosition(); // get mouse position
		ncScreenZoom -= GetMouseWheelMove() * 0.2f;
		ncScreenZoom = Clamp(ncScreenZoom, 0.1f, 10);
		UpdateEditor(position);


		if (ncEditorData.Reset) 
		{
			DestroyAllBodies(); 
			DestroyAllSprings();
			ncEditorData.Reset = false;
		}

		if (!ncEditorIntersect)
		{
			selectedBody = GetBodyIntersect(ncBbodies, position);
			if (selectedBody)
			{
				Vector2 screen = ConvertWorldToScreen(selectedBody->position);
				DrawCircleLines((int)screen.x, (int)screen.y, ConvertWorldToPixel(selectedBody->mass * 0.5f) + 5, YELLOW);
			}

			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || IsMouseButtonDown(MOUSE_BUTTON_LEFT) && IsKeyDown(KEY_W)) // if left mouse button is down
			{
				// Add a glow where clicked
				for (int i = 0; i < MAX_GLOWS; i++) {
					if (glowTimers[i] <= 0) { 
						glows[i].position = position; 
						glows[i].color = ncEditorData.BodyColor;
						glows[i].radius = ncEditorData.MassMinValue * 30; 
						glows[i].alpha = 1.0f; 
						glowTimers[i] = 3.0f; // Set glow timer 

						break;
					}
				}

				// Randomly generate firework
				for (int i = 0; i < 1; i++)
				{
					ncBody* body = CreateBody(ConvertScreenToWorld(position), ncEditorData.MassMinValue, ncEditorData.BodyTypeActive);
					body->damping = ncEditorData.DampingValue;       // how slow it will stop/fall, higher = slower 
					body->gravityScale = ncEditorData.GravityScaleValue;   // faster it falls
					body->restitution = ncEditorData.RestitutionValue;
					body->color = bodyC; // Use the same color for all particles

					AddBody(body);
				}
			}
		}

		// connec spring
		if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) && selectedBody) connectBody = selectedBody;
		if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT) && connectBody) DrawLineBodyToPosition(connectBody, position);
		if(connectBody)
		{
			Vector2 world = ConvertScreenToWorld(position); 
			if (connectBody->type == BT_STATIC || connectBody->type == BT_KINEMATIC)
			{
				connectBody->position = world;
			}
			else
			{
				ApplySpringForcePosition(world, connectBody, 0, ncEditorData.StiffnessValue, 5); 
			}
		}
		if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT) && connectBody) 
		{
			if (selectedBody && selectedBody != connectBody) 
			{
				ncSpring_t* spring = CreateSpring(connectBody, selectedBody, Vector2Distance(connectBody->position, selectedBody->position), ncEditorData.StiffnessValue); 
				AddSpring(spring); 
			}
		}
		if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT))
		{
			selectedBody = NULL; 
			connectBody = NULL; 
		}

		timeAccumulator += dt;

		while (timeAccumulator >= ncEditorData.FixedTimeStep)
		{
			timeAccumulator -= ncEditorData.FixedTimeStep;

			// Update glow timers
			for (int i = 0; i < MAX_GLOWS; i++) {
				if (glowTimers[i] > 0)
					glowTimers[i] -= dt;
			}

			if (!ncEditorData.SimulationNotActive)
			{
				// apply force
				ApplyGravitation(ncBbodies, ncEditorData.GravitationValue);
				ApplySpringForce(ncSprings);

				// Update particles 
				for (ncBody* body = ncBbodies; body; body = body->next)
				{
					Step(body, timeAccumulator);

					//// Apply trail fade rate
					body->color = Fade(body->color, TRAIL_FADE_RATE * 255);

					//// Shrink particle size
					if (ncEditorData.MassShrink)
					{
						body->mass -= body->mass * TRAIL_FADE_RATE; 
					}

					// Update the trail array
					for (int i = TRAIL_LENGTH - 1; i > 0; i--)
					{
						body->trail[i] = body->trail[i - 1];
					}
					body->trail[0] = body->position;

					// Draw particle
					Vector2 screen = ConvertWorldToScreen(body->position);
					DrawCircle(screen.x, screen.y, ConvertWorldToPixel(body->mass * 0.5f), body->color);
				}

				// collision
				DestroyAllContacts(&contacts); 
				CreateContacts(ncBbodies, &contacts);
				SeparateContacts(contacts);
				ResolveContacts(contacts);
			}
			else
			{
				continue;
			}
		}

		// render / draw
		BeginDrawing(); // begin drawing 
		ClearBackground(BLACK); // clear the background 

		// Render trails
		for (ncBody* particle = ncBbodies; particle; particle = particle->next)  
		{
			for (int i = 0; i < TRAIL_LENGTH - 1; i++)  
			{ 
				if (!Vector2IsZero(particle->trail[i]) && !Vector2IsZero(particle->trail[i + 1]))  
				{
					// Draw the fading line with proper fading effect
					DrawFadingLine(particle->trail[i], particle->trail[i + 1], particle->color, (float)(TRAIL_LENGTH - i) / (float)TRAIL_LENGTH, TRAIL_LENGTH / 2);  
				}
			}
		}

		// Draw glows
		for (int i = 0; i < MAX_GLOWS; i++) { 
			if (glowTimers[i] > 0) { 
				glows[i].radius += 50.0f * dt; // Increase radius 
				glows[i].alpha -= 0.5f * dt; // Decrease alpha 
				if (glows[i].alpha <= 0) 
					glowTimers[i] = 0; // Stop rendering glow 
				DrawFadingGlow(glows[i].position, glows[i].radius, glows[i].color, glows[i].alpha); 
			}
		}

		// draw stats
		DrawText(TextFormat("FPS: %.2f (%.2f ms)", fps, 1000 / fps), 10, 10, 20, LIME); // draw FPS
		DrawText(TextFormat("FRAME: %.4f", ncEditorData.FixedTimeStep), 10, 30, 20, LIME); // draw frame time

		//DrawCircle((int)position.x, (int)position.y, 5, YELLOW); // draw mouse cursor

		// draw bodies
		// Draw glowing bodies
		BeginBlendMode(BLEND_ADDITIVE);  
		for (ncBody* body = ncBbodies; body; body = body->next)  
		{
			Vector2 screen = ConvertWorldToScreen(body->position);
			DrawCircle((int)screen.x, (int)screen.y, ConvertWorldToPixel(body->mass * 0.5f), body->color);
		}
		EndBlendMode();  // End blending

		// Draw Contacts
		for (ncContact_t* contact = contacts; contact; contact = contact->next)
		{
			Vector2 screen = ConvertWorldToScreen(contact->body1->position);
			DrawCircle((int)screen.x, (int)screen.y, ConvertWorldToPixel(contact->body1->mass * 0.5f), RED);
		}

		// draw springs
		for (ncSpring_t* spring = ncSprings; spring; spring = spring->next)
		{
			Vector2 screen1 = ConvertWorldToScreen(spring->body1->position); 
			Vector2 screen2 = ConvertWorldToScreen(spring->body2->position);
			DrawLine((int)screen1.x, (int)screen1.y, (int)screen2.x, (int)screen2.y, YELLOW);
		}

		DrawEditor(position);

		EndDrawing(); // end drawing
	}

	CloseWindow(); // close the window

	return 0; // return from main function
}
