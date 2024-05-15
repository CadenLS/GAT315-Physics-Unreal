#include "body.h"
#include "integrator.h"
#include "world.h"

void Step(ncBody* body, float timestep)
{
	// Update the trail array
	for (int i = TRAIL_LENGTH - 1; i > 0; i--) 
	{ 
		body->trail[i] = body->trail[i - 1]; 
	}
	body->trail[0] = body->position; 

	body->force = Vector2Add(body->force, Vector2Scale(Vector2Scale(ncGravity, body->gravityScale), body->mass));
	body->acceleration = Vector2Scale(body->force, body->inverseMass);

	SemiImplicitEuler(body, timestep);

	// apply damping
	float damping = 1 / (1 + (body->damping * timestep));
	body->velocity = Vector2Scale(body->velocity, damping);

	ClearForce(body);
}