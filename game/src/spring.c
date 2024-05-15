#include "spring.h"
#include "body.h"
#include "editor.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h>

ncSpring_t* ncSprings = NULL;

ncSpring_t* CreateSpring(struct ncBody* body1, struct ncBody* body2, float restLength, float k)
{
	//Allocate memory for new Body
	ncSpring_t* spring = (ncSpring_t*)malloc(sizeof(ncSpring_t));
	//Check if allocation is successful
	assert(spring);

	memset(spring, 0, sizeof(ncSpring_t));
	spring->body1 = body1;
	spring->body2 = body2;
	spring->restLength = restLength;
	spring->k = k;

	//Return new Body
	return spring;
}

void AddSpring(ncSpring_t* spring)
{
	assert(spring);

	//Initialize 'prev' to NULL and 'next' to the head of the list
	spring->prev = NULL;
	spring->next = ncSprings;
	//If list is not empty, update 'prev' of existing head
	if (ncSprings)
	{
		ncSprings->prev = spring;
	}
	//Update head of the list to new Body
	ncSprings = spring;
}

void DestroySpring(ncSpring_t* spring)
{
	//Assert if provided Body is not NULL
	assert(spring);
	//If 'prev' is not NULL, set 'prev->next' to 'body->next'
	if (spring->prev) spring->prev->next = spring->next;
	//If 'next' is not NULL, set 'next->prev' to 'body->prev'
	if (spring->next) spring->next->prev = spring->prev;

	//If body is the head, update head to 'body->next'
	if (spring == ncSprings) ncSprings = spring->next;
	//Free the body
	free(spring);
}

void DestroyAllSprings()
{
}

void ApplySpringForce(ncSpring_t* springs)
{
	for (ncSpring_t* spring = springs; spring; spring = spring->next)
	{
		Vector2 direction = Vector2Subtract(spring->body1->position, spring->body2->position);  //<get direction vector from body2 to body1>
		if (direction.x == 0 && direction.y == 0) continue;

		float length = Vector2Length(direction); //<get length from direction>
		float x = length - spring->restLength; //<compute displacement from current length to resting length>;
		float force = -spring->k * x; //<(f = -kx) compute force using product of displacement and stiffness(k)>;

		Vector2 ndirection = Vector2Normalize(direction); //<get direction normal>

		//ApplyForce(spring->body1, <apply force>, FM_FORCE);
		ApplyForce(spring->body1, Vector2Scale(ndirection, force), ncEditorData.ForceModeActive);
		//ApplyForce(spring->body2, <apply negative force>, FM_FORCE);
		ApplyForce(spring->body2, Vector2Scale(Vector2Negate(ndirection), force), ncEditorData.ForceModeActive);
	}
}