#include "render.h"
#include "raymath.h"

// Define screen size and zoom factor
Vector2 ncScreenSize = { 1280, 720 };
float ncScreenZoom = 1.0f;

// Define view size
float ncViewSize = 25.0f;

// Function to convert screen coordinates to world coordinates
Vector2 ConvertScreenToWorld(Vector2 screen)
{
    // Calculate the aspect ratio
    float ratio = ncScreenSize.x / ncScreenSize.y;

    // Calculate the extents of the view
    Vector2 extents = { ncViewSize * ratio, ncViewSize };
    extents = Vector2Scale(extents, ncScreenZoom);

    // Calculate the lower and upper bounds of the view
    Vector2 lower = Vector2Negate(extents);
    Vector2 upper = extents;

    // Normalize the screen coordinates
    float nx = screen.x / ncScreenSize.x;
    float ny = (ncScreenSize.y - screen.y) / ncScreenSize.y;

    // Map normalized screen coordinates to world coordinates
    Vector2 world;
    world.x = ((1 - nx) * lower.x) + (nx * upper.x);
    world.y = ((1 - ny) * lower.y) + (ny * upper.y);

    return world; // Return world coordinates
}

// Function to convert world coordinates to screen coordinates
Vector2 ConvertWorldToScreen(Vector2 world)
{
    // Calculate the aspect ratio
    float ratio = ncScreenSize.x / ncScreenSize.y;

    // Calculate the extents of the view
    Vector2 extents = { ncViewSize * ratio, ncViewSize };
    extents = Vector2Scale(extents, ncScreenZoom);

    // Calculate the lower and upper bounds of the view
    Vector2 lower = Vector2Negate(extents);
    Vector2 upper = extents;

    // Normalize the world coordinates
    float nx = (world.x - lower.x) / (upper.x - lower.x);
    float ny = (world.y - lower.y) / (upper.y - lower.y);

    // Map normalized world coordinates to screen coordinates
    Vector2 screen;
    screen.x = nx * ncScreenSize.x;
    screen.y = (1.0f - ny) * ncScreenSize.y;

    return screen; // Return screen coordinates
}

// Function to convert world units to pixels
float ConvertWorldToPixel(float world)
{
    // Calculate the aspect ratio
    float ratio = ncScreenSize.x / ncScreenSize.y;

    // Calculate the extents of the view
    Vector2 extents = { ncViewSize * ratio, ncViewSize };
    extents = Vector2Scale(extents, ncScreenZoom);

    // Normalize the world unit
    float nx = world / extents.x;

    // Convert normalized world unit to pixels
    float pixel = nx * ncScreenSize.x;

    return pixel; // Return pixel value
}
