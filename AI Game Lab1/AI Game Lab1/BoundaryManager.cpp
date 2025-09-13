#include "BoundaryManager.h"

float BoundaryManager::s_screenWidth = 1000.0f;
float BoundaryManager::s_screenHeight = 800.0f;

void BoundaryManager::wrapPosition(sf::Vector2f& position, float screenWidth, float screenHeight, float buffer)
{
    // Smart checking - only do expensive wrapping when near boundaries
    const float boundaryThreshold = 50.0f; // How close to edge before we start checking

    bool isNearBoundary = (position.x < boundaryThreshold ||
        position.x > screenWidth - boundaryThreshold ||
        position.y < boundaryThreshold ||
        position.y > screenHeight - boundaryThreshold);

    if (!isNearBoundary) {
        return; // Far from edges, skip wrapping entirely
    }

    // Only do wrapping checks when actually needed
    if (position.x > screenWidth + buffer) {
        position.x = -buffer;
    }
    else if (position.x < -buffer) {
        position.x = screenWidth + buffer;
    }

    if (position.y > screenHeight + buffer) {
        position.y = -buffer;
    }
    else if (position.y < -buffer) {
        position.y = screenHeight + buffer;
    }
}

void BoundaryManager::setGlobalScreenSize(float width, float height)
{
    s_screenWidth = width;
    s_screenHeight = height;
}

void BoundaryManager::wrapPositionGlobal(sf::Vector2f& position, float buffer)
{
    wrapPosition(position, s_screenWidth, s_screenHeight, buffer);
}