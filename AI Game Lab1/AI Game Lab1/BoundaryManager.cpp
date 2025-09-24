#include "BoundaryManager.h"

// Initialize static members
float BoundaryManager::s_screenWidth = 1000.f;
float BoundaryManager::s_screenHeight = 800.f;

// Set global screen size
void BoundaryManager::setGlobalScreenSize(float width, float height) {
    s_screenWidth = width;
    s_screenHeight = height;
}

// Wrap a position inside arbitrary bounds
void BoundaryManager::wrapPosition(sf::Vector2f& position, float screenWidth, float screenHeight, float buffer) {
    const float boundaryThreshold = 50.f;

    bool isNearBoundary = (position.x < boundaryThreshold ||
                           position.x > screenWidth - boundaryThreshold ||
                           position.y < boundaryThreshold ||
                           position.y > screenHeight - boundaryThreshold);

    if (!isNearBoundary) return;
    // Only do calculations if we are actually near the boundary

    if (position.x > screenWidth + buffer) position.x = -buffer;
    else if (position.x < -buffer) position.x = screenWidth + buffer;

    if (position.y > screenHeight + buffer) position.y = -buffer;
    else if (position.y < -buffer) position.y = screenHeight + buffer;
}

// Wrap a position inside global screen bounds
void BoundaryManager::wrapPositionGlobal(sf::Vector2f& position, float buffer) {
    wrapPosition(position, s_screenWidth, s_screenHeight, buffer);
}

// Get global screen size
sf::Vector2f BoundaryManager::getGlobalScreenSize() {
    return sf::Vector2f(s_screenWidth, s_screenHeight);
}

