#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "NPC.h"
#include "Behaviours.h"

class NPCManager
{
public:
    NPCManager() = default;

    NPCManager(MovementContext& context);

    void createNPCs(const MovementContext& context);

    void updateAll(float dt);

    void render(sf::RenderWindow& window);

    void toggleNPC(size_t index);

    std::vector<std::unique_ptr<NPC>>& getAll();

private:
    MovementContext& m_context;
    std::vector<std::unique_ptr<NPC>> m_npcs;
};
