#pragma once
#include "world.h"
#include "body.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h>

ncBody* ncBbodies = NULL;
int ncBodyCount = 0;
Vector2 ncGravity;
Color bodyC;

ncBody* CreateBody(Vector2 position, float mass, ncBodyType bodyType)
{
	//Allocate memory for new Body
	ncBody* body = (ncBody*)malloc(sizeof(ncBody));
	//Check if allocation is successful
	assert(body);

	memset(body, 0, sizeof(ncBody));
	body->position = position;
	body->mass = mass;
	body->inverseMass = (bodyType == BT_DYNAMIC) ? 1 / body->mass : 0;
	body->type = bodyType;

	//Return new Body
	return body;
}

void AddBody(ncBody* body)
{
	assert(body);

	//Initialize 'prev' to NULL and 'next' to the head of the list
	body->prev = NULL; 
	body->next = ncBbodies; 
	//If list is not empty, update 'prev' of existing head
	if (ncBbodies) 
	{
		ncBbodies->prev = body; 
	}
	//Update head of the list to new Body
	ncBbodies = body; 
	//Increment body count
	ncBodyCount++; 
}

void DestroyBody(ncBody* body)
{
	//Assert if provided Body is not NULL
	assert(body);
	//If 'prev' is not NULL, set 'prev->next' to 'body->next'
	if (body->prev)
	{
		body->prev->next = body->next;
	}
	//If 'next' is not NULL, set 'next->prev' to 'body->prev'
	if (body->next)
	{
		body->next->prev = body->prev;
	}
	//If body is the head, update head to 'body->next'
	if (body == ncBbodies)
	{
		ncBbodies = body->next;
	}
	//Decrement body count
	ncBodyCount--;
	//Free the body
	free(body);
}

void DestroyAllBodies()
{
	if (!ncBbodies) return;

	ncBody* body = ncBbodies;
	while (body)
	{
		ncBody* next = body->next;
		free(body);
		body = next;
	}

	ncBbodies = NULL;
}