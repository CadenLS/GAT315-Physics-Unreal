#pragma once
//#include "body.h"
#include "raylib.h"

typedef struct ncBody ncBody; // forward declearation

extern ncBody* ncBbodies; // extern - prevents it from being defined outside of here
extern int ncBodyCount;
extern Vector2 ncGravity;

ncBody* CreateBody();
void DestroyBody(ncBody* body);
void DestroyAllBodies();