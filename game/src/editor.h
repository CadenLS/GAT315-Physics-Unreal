#pragma once
#include "raylib.h"
#include <string.h>

typedef struct ncEditorData
{
	bool EditorBoxActive;
	float MassMinValue;
	float GravitationValue;
	bool BodyTypeEditMode;
	int BodyTypeActive;
	bool ForceModeEditMode;
	int ForceModeActive;
	float GravityValue;
	float DampingValue;
	float GravityScaleValue;
	float StiffnessValue;
} ncEditorData_t;

typedef enum
{
	AMBER,
	ASHES,
	BLUISH,
	CANDY,
	CHERRY,
	CYBER,
	DARK,
	LIGHT,
	JUNGLE,
	ENEFETE,
	LAVANDA,
	SUNNY,
	TERMINAL
} ncEditorStyles;

extern ncEditorData_t ncEditorData;

extern bool ncEditorActive;
extern bool ncEditorIntersect;

void InitEditor();
void UpdateEditor(Vector2 position);
void DrawEditor(Vector2 position);

struct ncBody* GetBodyIntersect(struct ncBody* bodies, Vector2 position); 
void DrawLineBodyToPosition(struct ncBody* body, Vector2 position);