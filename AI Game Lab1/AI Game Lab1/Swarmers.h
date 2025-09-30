#pragma once

#include <SFML/Graphics.hpp>

struct Swarmer {
    sf::Vector2f m_position;
    sf::Vector2f m_velocity;
    float m_speed;
    sf::CircleShape m_body;

    Swarmer(sf::Vector2f t_pos, float t_radius)
        : m_position(t_pos), m_velocity(0.f, 0.f), m_body(t_radius), m_speed(1.0f)
    {
        m_body.setOrigin(sf::Vector2f(t_radius, t_radius));  // Center the Origin
        m_body.setPosition(t_pos);     // Uses a random position and sets it
        m_body.setFillColor(sf::Color::Red);
        m_body.setOutlineThickness(1.5f);
        m_body.setOutlineColor(sf::Color::White);
    }
};
