#pragma once
#include <SFML/Graphics.hpp>

class BoundaryManager 
{
public:
    // Set global screen dimensions
    static void setGlobalScreenSize(float width, float height);

    // Wrap position within global screen bounds
    static void wrapPositionGlobal(sf::Vector2f& position, float buffer = 0.f);

    // Wrap position within arbitrary bounds
    static void wrapPosition(sf::Vector2f& position, float screenWidth, float screenHeight, float buffer = 0.f);

    // Return global screen size
    static sf::Vector2f getGlobalScreenSize();

private:
    static float s_screenWidth;
    static float s_screenHeight;
};
