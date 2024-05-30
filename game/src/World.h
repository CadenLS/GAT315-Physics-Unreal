#pragma once
#include "body.h"
#include "raylib.h"

typedef struct ncBody ncBody; // forward declearation

extern ncBody* ncBbodies; // extern - prevents it from being defined outside of here
extern int ncBodyCount;
extern Vector2 ncGravity;
extern Color bodyC;

ncBody* CreateBody(Vector2 position, float mass, ncBodyType bodyType);
void AddBody(ncBody* body);
void DestroyBody(ncBody* body);
void DestroyAllBodies();