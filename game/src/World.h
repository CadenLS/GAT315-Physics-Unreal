#pragma once
#include "body.h"

extern Body* bodies; // extern - prevents it from being defined outside of here
extern int bodyCount;

Body* CreateBody();
void DestroyBody(Body body);