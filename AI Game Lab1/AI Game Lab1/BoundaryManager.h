#pragma once
#include <SFML/Graphics.hpp>

class BoundaryManager
{
public:
    static void wrapPosition(sf::Vector2f& position, float screenWidth, float screenHeight, float buffer = 20.0f);
    static void setGlobalScreenSize(float width, float height);
    static void wrapPositionGlobal(sf::Vector2f& position, float buffer = 20.0f); // Uses global screen size

private:
    static float s_screenWidth;
    static float s_screenHeight;
};