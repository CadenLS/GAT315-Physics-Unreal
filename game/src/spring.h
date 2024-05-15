#pragma once

typedef struct ncSpring
{
	struct ncBody* body1;
	struct ncBody* body2;
	float restLength;
	float k; // stiffness

	struct ncSpring* next;
	struct ncSpring* prev;
} ncSpring_t;

extern ncSpring_t* ncSprings;

ncSpring_t* CreateSpring(struct ncBody* body1, struct ncBody* body2, float restLength, float k);
void AddSpring(ncSpring_t* body);
void DestroySpring(ncSpring_t* body);
void DestroyAllSprings();

void ApplySpringForce(ncSpring_t* springs); // pass it to the head