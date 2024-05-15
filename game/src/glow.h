#pragma once
#include "raymath.h"
#include <raylib.h>

typedef struct {
	Vector2 position;
	Color color;
	float radius;
	float alpha;
} Glow;

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
