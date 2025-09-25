#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include "BoundaryManager.h"
#include "Player.h"

struct PlayerContext 
{
   const Player& player;

    // Constructor takes references to the actual Player data
    PlayerContext(const Player& p) : player(p) {}

    const sf::Vector2f& getPlayerPos() const { return player.getPosition(); }
    const sf::Vector2f& getPlayerVel() const { return player.getVelocity(); }

};

