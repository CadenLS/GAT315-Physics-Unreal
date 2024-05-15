#include "editor.h"
#include "body.h"
#include "render.h"
#define RAYGUI_IMPLEMENTATION
#include "../../raygui/src/raygui.h"

bool ncEditorActive = true;
bool ncEditorIntersect = false;
ncEditorData_t ncEditorData;
ncEditorStyles ncStyle = DARK; // Set default style
ncEditorStyles prevStyle = DARK;

Rectangle editorRect;

Vector2 anchor01 = { 1500, 100 };
Texture2D cursorTexture;

bool EditorActive = true;

// Define a mapping between ncEditorStyles enums and style file paths
const char* stylePaths[] = {
    "raygui/styles/amber/style_amber.rgs",
    "raygui/styles/ashes/style_ashes.rgs",
    "raygui/styles/bluish/style_bluish.rgs",
    "raygui/styles/candy/style_candy.rgs",
    "raygui/styles/cherry/style_cherry.rgs",
    "raygui/styles/cyber/style_cyber.rgs",
    "raygui/styles/dark/style_dark.rgs",
    "raygui/styles/default/style_default.old.rgs",
    "raygui/styles/jungle/style_jungle.rgs",
    "raygui/styles/enefete/style_enefete.rgs",
    "raygui/styles/lavanda/style_lavanda.rgs",
    "raygui/styles/sunny/style_sunny.rgs",
    "raygui/styles/terminal/style_terminal.rgs"
};

void InitEditor()
{
    // Load the style according to the default style
    //GuiLoadStyle(stylePaths[ncStyle]);
    GuiLoadStyle("raygui/styles/dark/style_dark.rgs");

    Image image = LoadImage("resources/reticle.png");
    cursorTexture = LoadTextureFromImage(image);
    UnloadImage(image);
    HideCursor();

    ncEditorData.GravitationValue = 2;
    ncEditorData.MassMinValue = 2.0f;
    ncEditorData.BodyTypeEditMode = false;  
    ncEditorData.BodyTypeActive = 0; 
    ncEditorData.ForceModeEditMode = false; 
    ncEditorData.ForceModeActive = 0; 
    ncEditorData.GravityValue = 0.0f;
    ncEditorData.DampingValue = 0.0f;
    ncEditorData.GravityScaleValue = 1.0f;
    ncEditorData.StiffnessValue = 10.0f;

    editorRect = (Rectangle){ anchor01.x - 120, anchor01.y, 456, 576 };
}

void UpdateEditor(Vector2 position)
{
    // toggle show/hide editor box with key press
    if (IsKeyPressed(KEY_TAB)) ncEditorData.EditorBoxActive = !ncEditorData.EditorBoxActive;

    // check if cursor position is intersecting the editor box
    ncEditorIntersect = ncEditorData.EditorBoxActive && CheckCollisionPointRec(position, editorRect);
}

void DrawEditor(Vector2 position)
{
    if (ncEditorData.BodyTypeEditMode) GuiLock();

    if (ncEditorData.EditorBoxActive)
    {
        EditorActive = !GuiWindowBox((Rectangle) { anchor01.x - 120, anchor01.y, 456, 576 }, "Editor");
        GuiSliderBar((Rectangle) { anchor01.x + 120, anchor01.y + 120, 120, 16 }, "Mass", TextFormat("%0.2f", ncEditorData.MassMinValue), & ncEditorData.MassMinValue, 0.1f, 5);
        GuiSlider((Rectangle) { anchor01.x + 120, anchor01.y + 400, 120, 16 }, "Gravitation", TextFormat("%0.2f", ncEditorData.GravitationValue), & ncEditorData.GravitationValue, -20, 20);
        GuiGroupBox((Rectangle) { anchor01.x + 16.5f, anchor01.y + 40, 304, 224 }, "Bodies");
        GuiGroupBox((Rectangle) { anchor01.x + 16.5f, anchor01.y + 320, 304, 232 }, "World");
        GuiSlider((Rectangle) { anchor01.x + 120, anchor01.y + 360, 120, 16 }, "Gravity", TextFormat("%0.2f", ncEditorData.GravityValue), & ncEditorData.GravityValue, -10, 10);
        GuiSliderBar((Rectangle) { anchor01.x + 120, anchor01.y + 160, 120, 16 }, "Damping", TextFormat("%0.2f", ncEditorData.DampingValue), & ncEditorData.DampingValue, 0, 40);
        GuiSliderBar((Rectangle) { anchor01.x + 120, anchor01.y + 200, 120, 16 }, "Gravity Scale", TextFormat("%0.2f", ncEditorData.GravityScaleValue), & ncEditorData.GravityScaleValue, 0, 10);
        GuiSliderBar((Rectangle) { anchor01.x + 120, anchor01.y + 240, 120, 16 }, "Stiffness", TextFormat("%0.2f", ncEditorData.StiffnessValue), & ncEditorData.StiffnessValue, 0, 40);
        if (GuiDropdownBox((Rectangle) { anchor01.x + 40, anchor01.y + 60, 120, 24 }, "BT_DYNAMIC;BT_KINEMATIC;BT_STATIC", & ncEditorData.BodyTypeActive, ncEditorData.BodyTypeEditMode)) ncEditorData.BodyTypeEditMode = !ncEditorData.BodyTypeEditMode;
        if (GuiDropdownBox((Rectangle) { anchor01.x + 180, anchor01.y + 60, 120, 24 }, "FM_FORCE;FM_IMPULSE;FM_VELOCITY", & ncEditorData.ForceModeActive, ncEditorData.ForceModeEditMode)) ncEditorData.ForceModeEditMode = !ncEditorData.ForceModeEditMode;
        //int i = GuiDropdownBox((Rectangle) { anchor01.x - 110, anchor01.y + 60, 120, 24 }, "AMBER;ASHES;BLUISH;CANDY;CHERRY;CYBER;DARK;LIGHT;JUNGLE;ENEFETE;LAVANDA;SUNNY;TERMINAL", & ncStyle, true);
        //printf("%d\n", i);
        if (GuiDropdownBox((Rectangle) { anchor01.x - 110, anchor01.y + 60, 120, 24 }, "AMBER;ASHES;BLUISH;CANDY;CHERRY;CYBER;DARK;LIGHT;JUNGLE;ENEFETE;LAVANDA;SUNNY;TERMINAL", & ncStyle, true)) 
        {
            if (ncStyle != prevStyle) GuiLoadStyle(stylePaths[ncStyle]); 
        }
        prevStyle = ncStyle;
    }

    DrawTexture(cursorTexture, (int)position.x - cursorTexture.width / 2, (int)position.y - cursorTexture.height / 2, WHITE);

    GuiUnlock();
}

ncBody* GetBodyIntersect(ncBody* bodies, Vector2 position)
{
    for (ncBody* body = bodies; body; body = body->next)
    {
        Vector2 screen = ConvertWorldToScreen(body->position);
        if (CheckCollisionPointCircle(position, screen, ConvertWorldToPixel(body->mass * 0.5f)))
        {
            return body;
        }
    }

    return NULL;
}

void DrawLineBodyToPosition(ncBody* body, Vector2 position)
{
    Vector2 screen = ConvertWorldToScreen(body->position);
    DrawLine((int)screen.x, (int)screen.y, (int)position.x - cursorTexture.width / 2, (int)position.y - cursorTexture.height / 2, YELLOW);
}