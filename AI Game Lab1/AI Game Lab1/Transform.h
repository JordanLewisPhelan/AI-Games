#pragma once
#include "NPC.h"

class Transform
{
public:
    // Attached to an NPC (dynamic)
    Transform(NPC& npc)
        : m_npc(&npc), m_position(npc.getPosition()) {}


    void Update();

    // Accessors
    sf::Vector2f getPosition() const;

    void setPosition(const sf::Vector2f& pos);

private:
    NPC* m_npc = nullptr;             // attached NPC 
    sf::Vector2f m_position = { 0, 0 }; 
};
